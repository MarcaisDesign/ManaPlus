/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
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

#ifndef OPENGLGRAPHICS_H
#define OPENGLGRAPHICS_H

#ifdef USE_OPENGL

#include "localconsts.h"
#include "main.h"
#include "graphics.h"

#include "resources/fboinfo.h"

//#define NO_SDL_GLEXT
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
#include <GL/glext.h>

#include <set>

class NormalOpenGLGraphics : public Graphics
{
    public:
        NormalOpenGLGraphics();

        ~NormalOpenGLGraphics();

        bool setVideoMode(const int w, const int h, const int bpp,
                          const bool fs, const bool hwaccel,
                          const bool resize, const bool noFrame);


        /**
         * Draws a resclaled version of the image
         */
        bool drawRescaledImage(Image *const image, int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth, const int desiredHeight,
                               const bool useColor);

        /**
         * Used to get the smooth rescale option over the standard function.
         */
        bool drawRescaledImage(Image *const image, int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth, const int desiredHeight,
                               const bool useColor, bool smooth);

        void drawImagePattern(const Image *const image,
                              const int x, const int y,
                              const int w, const int h);

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        void drawRescaledImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const int scaledWidth,
                                      const int scaledHeight);

        void calcImagePattern(GraphicsVertexes *const vert,
                              const Image *const image,
                              const int x, const int y,
                              const int w, const int h);

        void calcTile(ImageVertexes *const vert, int x, int y);

        void drawTile(const ImageVertexes *const vert);

        void drawImagePattern2(GraphicsVertexes *const vert,
                               const Image *const image);

        void updateScreen();

        void _beginDraw();
        void _endDraw();

        bool pushClipArea(gcn::Rectangle area);
        void popClipArea();

        void setColor(const gcn::Color &color);

        void drawPoint(int x, int y);

        void drawLine(int x1, int y1, int x2, int y2);

        void drawRectangle(const gcn::Rectangle &rect, bool filled);

        void drawRectangle(const gcn::Rectangle &rect);

        void fillRectangle(const gcn::Rectangle &rect);

        void setTargetPlane(int width, int height);

        void drawQuadArrayfi(int size);

        void drawQuadArrayfi(GLint *intVertArray, GLfloat *floatTexArray,
                             int size);

        void drawQuadArrayii(int size);

        void drawQuadArrayii(GLint *intVertArray, GLint *intTexArray,
                             int size);

        void drawLineArrayi(int size);

        void drawLineArrayf(int size);

        void initArrays();

        static void dumpSettings();

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        SDL_Surface *getScreenshot();

        void prepareScreenshot();

        bool drawNet(const int x1, const int y1, const int x2, const int y2,
                     const int width, const int height);

        int getMemoryUsage();

        void updateTextureFormat();

        static void bindTexture(GLenum target, GLuint texture);

        static GLuint mLastImage;

    protected:
        bool drawImage2(const Image *const image,
                        int srcX, int srcY,
                        int dstX, int dstY,
                        const int width, const int height,
                        const bool useColor);

        void setTexturingAndBlending(bool enable);

        void logString(const char *format, GLenum num);

        void updateMemoryInfo();

        void debugBindTexture(const Image *image);

    private:
        void inline setColorAlpha(float alpha);

        void inline restoreColor();

        GLfloat *mFloatTexArray;
        GLint *mIntTexArray;
        GLint *mIntVertArray;
        bool mAlpha;
        bool mTexture;

        bool mIsByteColor;
        gcn::Color mByteColor;
        float mFloatColor;
        int mMaxVertices;
        bool mColorAlpha;
        FBOInfo mFbo;
#ifdef DEBUG_BIND_TEXTURE
        std::string mOldTexture;
        unsigned mOldTextureId;
#endif
};
#endif

extern unsigned int vertexBufSize;

#endif
