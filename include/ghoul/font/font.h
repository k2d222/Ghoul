/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2015                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __FONT_H__
#define __FONT_H__

#include <ghoul/glm.h>
#include <ghoul/misc/dictionary.h>
#include <ghoul/opengl/textureatlas.h>

#include <string>
#include <vector>

namespace ghoul {
namespace fontrendering {

/**
 * The Font class encapsulates a single fontface for a specific font size. It contains all
 * the information that is necessary to compute display sizes and, using the FontRendering
 * render the font to the screen. Each Font consists of Glyphs, the individual characters.
 * A Font can have an outline, which is a border of varying thickness around each
 * character. Individual Glyphs can be requested using the #glyph function, which
 * automatically loads and caches missing Glyphs on the first load. The storage backend
 * for Fonts is TextureAtlas into which all Glyphs (regular and outline) are saved. Access
 * into this TextureAtlas is performed on a per-glyph basis and each Glyph stores its
 * texture coordinates in the atlas. Each Font is uniquely identified by the combination
 * of name, font size, whether it has an outline, the thickness of the outline, and the
 * TextureAtlas it uses.
 */
class Font {
public:
    /**
     * This class contains the metrics and the texture locations in the TextureAtlas for a
     * single glyph for a specific font. Each glyph supplies two pairs of coordinates:
     * </br>
     * 1. The top left and bottom right corners of the base glyph (i.e., the regular
     * glyph if it is rendered without an outline.</br>
     * 2. The top left and bottom right corners of the outline glyph (i.e., a filled glyph
     * that can be rendered behind the base glyph in a different color to provide an
     * outline to the base.
     */
    class Glyph {
    public:
        friend class Font;

        /// The default constructor for a Glyph
        Glyph(wchar_t character,
              int width = 0,
              int height = 0,
              int offsetX = 0,
              int offsetY = 0,
              float advanceX = 0.f,
              float advanceY = 0.f,
              glm::vec2 texCoordTopLeft = glm::vec2(0.f),
              glm::vec2 texCoordBottomRight = glm::vec2(0.f),
              glm::vec2 outlineTexCoordTopLeft = glm::vec2(0.f),
              glm::vec2 outlineTexCoordBottomRight = glm::vec2(0.f)
        );
        
        bool operator==(const Glyph& rhs) const;
        
        /**
         * Returns the horizontal extent of the glyph
         * \return The horizontal extent of the glyph
         */
        int width() const;
        
        /**
         * Returns the vertical extent of the glyph
         * \return The vertical extent of the glyph
         */
        int height() const;

        /**
         * Returns the left-side bearing of the glyph
         * \return the left-side bearing of the glyph
         */
        int offsetX() const;
        
        /**
         * Returns the top-side bearing of the glyph
         * \return The top-side bearing of the glyph
         */
        int offsetY() const;

        /**
         * Returns the horizontal advance for this glyph
         * \return The horizontal advance for this glyph
         */
        float horizontalAdvance() const;
        
        /**
         * Returns the vertical advance for this glyph
         * \return The vertical advance for this glyph
         */
        float verticalAdvance() const;
        
        /**
         * Returns the kerning value between this glyph and <code>character</code>.
         * \param character The following character for which the kerning value should be
         * returned.
         * \return The kerning value between this glyph and <code>character</code>.
         */
        float kerning(wchar_t character) const;
        
        /**
         * Returns the texture coordinate that points to the top left corner of the base
         * representation for this Glyph in the TextureAtlas
         * \return The top left base texture coordinate
         */
        const glm::vec2& topLeft() const;
        
        /**
         * Returns the texture coordinate that points to the bottom right corner of the
         * base representation for this Glyph in the TextureAtlas
         * \return The bottom right base texture coordinates
         */
        const glm::vec2& bottomRight() const;
        
        /**
         * Returns the texture coordinate that points to the top left corner of the
         * outline representation for this Glyph in the TextureAtlas
         * \return The top left outline texture coordinate
         */
        const glm::vec2& outlineTopLeft() const;
        
        /**
         * Returns the texture coordinate that points to the bottom right corner of the
         * outline representation for this Glyph in the TextureAtlas
         * \return The bottom right outline texture coordinate
         */
        const glm::vec2& outlineBottomRight() const;
        
    private:
        /// The wide character that this glyph represents
        wchar_t _charcode;

        /// Glyph's width in pixels
        int _width;

        /// Glyph's height in pixels
        int _height;

        ///< Glyph's left bearing expressed in pixels
        int _offsetX;

        /// Glyphs's top bearing expressed in pixels
        int _offsetY;

        /// This is the distance used when the glyph is drawn as part
        /// of horizontal text
        float _horizontalAdvance;
        
        /// This is the distance used when the glyph is drawn as part
        /// of vertical text
        float _verticalAdvance;
        
        /// Normalized texture coordinate of top-left corner
        glm::vec2 _topLeft;
        /// Normalized texture coordinate of bottom-right corner
        glm::vec2 _bottomRight;

        /// Normalized texture coordinates for the top left of the
        /// outline
        glm::vec2 _outlineTopLeft;

        /// Normalized texture coordinates for the bottom right of the
        /// outline
        glm::vec2 _outlineBottomRight;

        /// A vector of kerning pairs relative to this glyph
        std::map<wchar_t, float> _kerning;
    };

    /**
     * Constructor creating a new Font with the specified <code>filename</code> at the
     * provided <code>pointSize</code>. The Glyphs of this Font will be stored in the
     * <code>atlas</code> TextureAtlas if there is enough free space. If
     * <code>outline</code> is <code>true</code> two sets of Glyphs are created which are
     * combined to provide an outline of thickness <code>outlineThickness</code> to the
     * glyphs.
     * \param filename The full path to the font file
     * \param pointSize The font size in pt
     * \param atlas The TextureAtlas which holds the created Glyphs
     * \param hasOutline A flag whether Glyphs of this Font should have an outline or not
     * \param outlineThickness The thickness of the outline. This setting is ignored if
     * the Font does not have an outline.
     */
    Font(std::string filename,
         float pointSize,
         opengl::TextureAtlas& atlas,
         bool hasOutline = true,
         float outlineThickness = 1.f
    );

    /**
     * Initializes the Font by loading the <code>filename</code> provided in the
     * constructor and setting some Font metrics. Calling this function after the
     * contruction is the first step to test whether the Font works. The return value
     * reports whether an error has occured.
     * \return <code>true</code> if the Font has been initialized successfully,
     * <code>false</code> otherwise
     */
    bool initialize();

    /**
     * Returns the name of the Font
     * \return The name of the Font
     */
    std::string name() const;
    
    /**
     * Returns the font size of this Font
     * \return The font size of this Font
     */
    float pointSize() const;
    
    /**
     * Returns the line seperator for this Font. This is the vertical length that
     * separates two consecutive lines.
     * \return The vertical line separation
     */
    float height() const;
    
    /**
     * Returns whether this Font has an outline or not
     * \return <code>true</code> if this Font has an outline, <code>false</code> otherwise
     */
    bool hasOutline() const;
    
    /**
     * Returns the Glyph that representes the passed <code>character</code>. The first
     * call to this function for each character creates and caches the Glyph before
     * returning it.
     * \param character The character for which the Glyph should be returned
     * \return A pointer to the Glyph or <code>nullptr</code> if the Glyph could not be
     * loaded
     */
    const Glyph* glyph(wchar_t character);

    /**
     * Preloads a list of Glyphs. Glyphs that are passed as part of 
     * <code>characters</code> that have been loaded previously are ignored and not loaded 
     * multiple times.
     * \param characters A list of characters for which Glyphs should be created and
     * cached
     * \return The number of characters that have not been loaded. If this value is 0, all
     * passed characters were successfully loaded
     */
    size_t loadGlyphs(const std::vector<wchar_t>& characters);
    
    /**
     * Returns the TextureAtlas that stores all of the Glyphs for this Font
     * \return The TextureAtlas that stores all of the Glyphs for this Font
     */
    opengl::TextureAtlas& atlas();

private:
    /// Generates the Kerning values for all Glyph pairs that have sofar been loaded
    void generateKerning();

    /// A list of all loaded Glyphs
    std::vector<Glyph> _glyphs;
    
    /// The TextureAtlas backend storage for the loaded Glyphs
    opengl::TextureAtlas& _atlas;
    
    /// The file name of this Font
    std::string _name;
    
    /// The font size in pt
    float _pointSize;
    
    /// The vertical distance between two consecutive lines
    float _height;
    
    /// Whether this Font has an outline or not
    bool _hasOutline;
    
    /// The thickness of the outline
    float _outlineThickness;
};
    
} // namespace fontrendering
} // namespace ghoul

#endif // __FONT_H__
