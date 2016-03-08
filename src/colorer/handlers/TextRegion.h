#ifndef _COLORER_TEXTREGION_H_
#define _COLORER_TEXTREGION_H_

#include <colorer/Exception.h>
#include <colorer/handlers/RegionDefine.h>

/**
 * Contains information about region mapping into textual prefix/suffix.
 * These mappings are stored in HRD files.
 * @ingroup colorer_handlers
 */
class TextRegion : public RegionDefine
{
public:

  /**
   * Text wrapping information.
   * Pointers are managed externally.
   */
  const String* start_text;
  const String* end_text;
  const String* start_back;
  const String* end_back;

  /**
   * Initial constructor
   */
  TextRegion(const String* _stext, const String* _etext,
             const String* _sback, const String* _eback)
  {
    start_text = _stext;
    end_text = _etext;
    start_back = _sback;
    end_back = _eback;
    type = RegionDefine::TEXT_REGION;
  }

  TextRegion()
  {
    start_text = end_text = start_back = end_back = nullptr;
    type = RegionDefine::TEXT_REGION;
  }

  /**
   * Copy constructor.
   * Clones all values including region reference
   */
  TextRegion(const TextRegion &rd)
  {
    operator=(rd);
  }

  ~TextRegion() {}

  /**
   * Static method, used to cast RegionDefine class into TextRegion class.
   * @throw Exception If casing is not available.
   */
  static const TextRegion* cast(const RegionDefine* rd)
  {
    if (rd == nullptr) return nullptr;
    if (rd->type != RegionDefine::TEXT_REGION) {
      throw Exception(DString("Bad type cast exception into TextRegion"));
    }
    const TextRegion* tr = (const TextRegion*)(rd);
    return tr;
  }

  /**
   * Assigns region define with it's parent values.
   * All fields are to be replaced, if they are null-ed.
  */
  void assignParent(const RegionDefine* _parent)
  {
    const TextRegion* parent = TextRegion::cast(_parent);
    if (parent == nullptr) return;
    if (start_text == nullptr || end_text == nullptr) {
      start_text = parent->start_text;
      end_text = parent->end_text;
    }
    if (start_back == nullptr || end_back == nullptr) {
      start_back = parent->start_back;
      end_back = parent->end_back;
    }
  }

  /**
   * Direct assign of all passed @c rd values.
   * Do not assign region reference.
   */
  void setValues(const RegionDefine* _rd)
  {
    if (_rd == nullptr) return;
    const TextRegion* rd = TextRegion::cast(_rd);
    start_text = rd->start_text;
    end_text = rd->end_text;
    start_back = rd->start_back;
    end_back = rd->end_back;
    type  = rd->type;
  }

  RegionDefine* clone() const
  {
    RegionDefine* rd = new TextRegion(*this);
    return rd;
  }

};

#endif
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library.
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <irusskih at gmail dot com>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2009
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): see file CONTRIBUTORS
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

