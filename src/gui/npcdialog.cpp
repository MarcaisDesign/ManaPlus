/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/npcdialog.h"

#include "configuration.h"
#include "client.h"

#include "gui/setup.h"

#include "gui/widgets/browserbox.h"
#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/itemlinkhandler.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/textfield.h"

#include "net/net.h"
#include "net/npchandler.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

#define CAPTION_WAITING _("Waiting for server")
#define CAPTION_NEXT _("Next")
#define CAPTION_CLOSE _("Close")
#define CAPTION_SUBMIT _("Submit")

NpcDialog::DialogList NpcDialog::instances;

NpcDialog::NpcDialog(int npcId)
    : Window(_("NPC")),
      mNpcId(npcId),
      mLogInteraction(config.getBoolValue("logNpcInGui")),
      mDefaultInt(0),
      mInputState(NPC_INPUT_NONE),
      mActionState(NPC_ACTION_WAIT),
      mLastNextTime(0)
{
    // Basic Window Setup
    setWindowName("NpcText");
    setResizable(true);
    //setupWindow->registerWindowForReset(this);
    setFocusable(true);

    setMinWidth(200);
    setMinHeight(150);

    setDefaultSize(260, 200, ImageRect::CENTER);

    mItemLinkHandler = new ItemLinkHandler;
    // Setup output text box
    mTextBox = new BrowserBox;
    mTextBox->setOpaque(false);
    mTextBox->setLinkHandler(mItemLinkHandler);

    mScrollArea = new ScrollArea(mTextBox);
    mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mScrollArea->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);

    // Setup listbox
    mItemList = new ListBox(this);
    mItemList->setWrappingEnabled(true);
    setContentSize(260, 175);

    mListScrollArea = new ScrollArea(mItemList);
    mListScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

    mItemList->setVisible(true);

    // Setup string input box
    mTextField = new TextField("");
    mTextField->setVisible(true);

    // Setup int input box
    mIntField = new IntTextField;
    mIntField->setVisible(true);

    mClearButton = new Button(_("Clear"), "clear", this);

    // Setup button
    mButton = new Button("", "ok", this);

    //Setup more and less buttons (int input)
    mPlusButton = new Button(_("+"), "inc", this);
    mMinusButton = new Button(_("-"), "dec", this);

    int width = std::max(mButton->getFont()->getWidth(CAPTION_WAITING),
                         mButton->getFont()->getWidth(CAPTION_NEXT));
    width = std::max(width, mButton->getFont()->getWidth(CAPTION_CLOSE));
    width = std::max(width, mButton->getFont()->getWidth(CAPTION_SUBMIT));

    mButton->setWidth(8 + width);

    mResetButton = new Button(_("Reset"), "reset", this);

    // Place widgets
    buildLayout();

    center();
    loadWindowState();

    instances.push_back(this);
    setVisible(true);
    requestFocus();

    config.addListener("logNpcInGui", this);
}

NpcDialog::~NpcDialog()
{
    config.removeListener("logNpcInGui", this);

    // These might not actually be in the layout, so lets be safe
    delete mScrollArea;
    mScrollArea = 0;
    delete mItemList;
    mItemList = 0;
    delete mTextField;
    mTextField = 0;
    delete mIntField;
    mIntField = 0;
    delete mResetButton;
    mResetButton = 0;
    delete mPlusButton;
    mPlusButton = 0;
    delete mMinusButton;
    mMinusButton = 0;
    delete mItemLinkHandler;
    mItemLinkHandler = 0;

    instances.remove(this);
}

/*
void NpcDialog::setText(const std::string &text)
{
    mText = text;

    mTextBox->setTextWrapped(mText, mScrollArea->getWidth() - 15);
}
*/

void NpcDialog::addText(const std::string &text, bool save)
{
    if (save || mLogInteraction)
    {
        if (mText.size() > 5000)
            mText = "";

        mNewText += text;
        mTextBox->addRow(text);
//        setText(mText + text + "\n");
    }
    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
    mActionState = NPC_ACTION_WAIT;
    buildLayout();
}

void NpcDialog::showNextButton()
{
    mActionState = NPC_ACTION_NEXT;
    buildLayout();
}

void NpcDialog::showCloseButton()
{
    mActionState = NPC_ACTION_CLOSE;
    buildLayout();
}

void NpcDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok")
    {
        if (mActionState == NPC_ACTION_NEXT)
        {
            if (!Client::limitPackets(PACKET_NPC_NEXT))
                return;

            nextDialog();
            // TRANSLATORS: Please leave the \n sequences intact.
            addText(_("> Next"), false);
        }
        else if (mActionState == NPC_ACTION_CLOSE)
        {
            closeDialog();
        }
        else if (mActionState == NPC_ACTION_INPUT)
        {
            std::string printText = "";  // Text that will get printed
                                         // in the textbox

            if (mInputState == NPC_INPUT_LIST)
            {
                unsigned char choice = 0;
                int selectedIndex = mItemList->getSelected();

                if (selectedIndex >= static_cast<int>(mItems.size())
                    || selectedIndex < 0
                    || !Client::limitPackets(PACKET_NPC_INPUT))
                {
                    return;
                }
                choice = static_cast<unsigned char>(selectedIndex + 1);
                printText = mItems[selectedIndex];

                Net::getNpcHandler()->listInput(mNpcId, choice);
            }
            else if (mInputState == NPC_INPUT_STRING)
            {
                if (!Client::limitPackets(PACKET_NPC_INPUT))
                    return;
                printText = mTextField->getText();
                Net::getNpcHandler()->stringInput(mNpcId, printText);
            }
            else if (mInputState == NPC_INPUT_INTEGER)
            {
                if (!Client::limitPackets(PACKET_NPC_INPUT))
                    return;
                printText = strprintf("%d", mIntField->getValue());
                Net::getNpcHandler()->integerInput(
                    mNpcId, mIntField->getValue());
            }
            // addText will auto remove the input layout
            addText(strprintf("> \"%s\"", printText.c_str()), false);

            mNewText.clear();
        }

        if (!mLogInteraction)
            mTextBox->clearRows();
    }
    else if (event.getId() == "reset")
    {
        if (mInputState == NPC_INPUT_STRING)
            mTextField->setText(mDefaultString);
        else if (mInputState == NPC_INPUT_INTEGER)
            mIntField->setValue(mDefaultInt);
    }
    else if (event.getId() == "inc")
    {
        mIntField->setValue(mIntField->getValue() + 1);
    }
    else if (event.getId() == "dec")
    {
        mIntField->setValue(mIntField->getValue() - 1);
    }
    else if (event.getId() == "clear")
    {
        mTextBox->clearRows();
//        mTextBox->addRow(mNewText);
//        setText(mNewText);
    }
}

void NpcDialog::nextDialog()
{
    Net::getNpcHandler()->nextDialog(mNpcId);
}

void NpcDialog::closeDialog()
{
    Net::getNpcHandler()->closeDialog(mNpcId);
}

int NpcDialog::getNumberOfElements()
{
    return static_cast<int>(mItems.size());
}

std::string NpcDialog::getElementAt(int i)
{
    return mItems[i];
}

void NpcDialog::choiceRequest()
{
    mItems.clear();
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_LIST;
    buildLayout();
}

void NpcDialog::addChoice(const std::string &choice)
{
    mItems.push_back(choice);
}

void NpcDialog::parseListItems(const std::string &itemString)
{
    std::istringstream iss(itemString);

    std::string tmp;
    while (getline(iss, tmp, ':'))
        mItems.push_back(tmp);
}

void NpcDialog::textRequest(const std::string &defaultText)
{
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_STRING;
    mDefaultString = defaultText;
    mTextField->setText(defaultText);
    buildLayout();
}

bool NpcDialog::isTextInputFocused() const
{
    return mTextField->isFocused();
}

bool NpcDialog::isInputFocused() const
{
    return mTextField->isFocused() || mIntField->isFocused();
}

bool NpcDialog::isAnyInputFocused()
{
    DialogList::iterator it = instances.begin();
    DialogList::iterator it_end = instances.end();

    for (; it != it_end; ++it)
    {
        if ((*it)->isInputFocused())
            return true;
    }

    return false;
}

void NpcDialog::integerRequest(int defaultValue, int min, int max)
{
    mActionState = NPC_ACTION_INPUT;
    mInputState = NPC_INPUT_INTEGER;
    mDefaultInt = defaultValue;
    mIntField->setRange(min, max);
    mIntField->setValue(defaultValue);
    buildLayout();
}

void NpcDialog::move(int amount)
{
    if (mActionState != NPC_ACTION_INPUT)
        return;

    switch (mInputState)
    {
        case NPC_INPUT_INTEGER:
            mIntField->setValue(mIntField->getValue() + amount);
            break;
        case NPC_INPUT_LIST:
            mItemList->setSelected(mItemList->getSelected() - amount);
            break;
        case NPC_INPUT_NONE:
        case NPC_INPUT_STRING:
        default:
            break;
    }
}

void NpcDialog::widgetResized(const gcn::Event &event)
{
    Window::widgetResized(event);

//    setText(mText);
}

void NpcDialog::setVisible(bool visible)
{
    Window::setVisible(visible);

    if (!visible)
        scheduleDelete();
}

void NpcDialog::optionChanged(const std::string &name)
{
    if (name == "logNpcInGui")
        mLogInteraction = config.getBoolValue("logNpcInGui");
}

NpcDialog *NpcDialog::getActive()
{
    if (instances.size() == 1)
        return instances.front();

    DialogList::iterator it = instances.begin();
    DialogList::iterator it_end = instances.end();

    for (; it != it_end; ++it)
    {
        if ((*it)->isFocused())
            return (*it);
    }

    return 0;
}

void NpcDialog::closeAll()
{
    DialogList::iterator it = instances.begin();
    DialogList::iterator it_end = instances.end();

    for (; it != it_end; ++it)
    {
        if (*it)
            (*it)->close();
    }
}

void NpcDialog::buildLayout()
{
    clearLayout();

    if (mActionState != NPC_ACTION_INPUT)
    {
        if (mActionState == NPC_ACTION_WAIT)
            mButton->setCaption(CAPTION_WAITING);
        else if (mActionState == NPC_ACTION_NEXT)
            mButton->setCaption(CAPTION_NEXT);
        else if (mActionState == NPC_ACTION_CLOSE)
            mButton->setCaption(CAPTION_CLOSE);
        place(0, 0, mScrollArea, 5, 3);
        place(3, 3, mClearButton);
        place(4, 3, mButton);
    }
    else if (mInputState != NPC_INPUT_NONE)
    {
//        if (!mLogInteraction)
//            setText(mNewText);

        mButton->setCaption(CAPTION_SUBMIT);
        if (mInputState == NPC_INPUT_LIST)
        {
            place(0, 0, mScrollArea, 6, 3);
            place(0, 3, mListScrollArea, 6, 3);
            place(2, 6, mClearButton, 2);
            place(4, 6, mButton, 2);

            mItemList->setSelected(-1);
        }
        else if (mInputState == NPC_INPUT_STRING)
        {
            place(0, 0, mScrollArea, 6, 3);
            place(0, 3, mTextField, 6);
            place(0, 4, mResetButton, 2);
            place(2, 4, mClearButton, 2);
            place(4, 4, mButton, 2);
        }
        else if (mInputState == NPC_INPUT_INTEGER)
        {
            place(0, 0, mScrollArea, 6, 3);
            place(0, 3, mMinusButton, 1);
            place(1, 3, mIntField, 4);
            place(5, 3, mPlusButton, 1);
            place(0, 4, mResetButton, 2);
            place(2, 4, mClearButton, 2);
            place(4, 4, mButton, 2);
        }
    }

    Layout &layout = getLayout();
    layout.setRowHeight(0, Layout::AUTO_SET);

    mButton->setEnabled(mActionState != NPC_ACTION_WAIT);

    redraw();

    mScrollArea->setVerticalScrollAmount(mScrollArea->getVerticalMaxScroll());
}