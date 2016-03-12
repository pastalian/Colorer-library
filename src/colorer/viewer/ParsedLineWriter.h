#ifndef _COLORER_PARSEDLINEWRITER_H_
#define _COLORER_PARSEDLINEWRITER_H_

#include<colorer/io/Writer.h>
#include<colorer/handlers/LineRegion.h>
/**
    Static service methods of LineRegion output.
    @ingroup colorer_viewer
*/
class ParsedLineWriter{
public:


  /** Writes given line of text using list of passed line regions.
      Formats output with class of each token, enclosed in
      \<span class='regionName'>...\</span>
      @param markupWriter Writer, used for markup output
      @param textWriter Writer, used for text output
      @param line Line of text
      @param lineRegions Linked list of LineRegion structures.
             Only region references are used there.
  */
  static void tokenWrite(Writer *markupWriter, Writer *textWriter, std::unordered_map<SString, String*> *docLinkHash, String *line, LineRegion *lineRegions){
    int pos = 0;
    for(LineRegion *l1 = lineRegions; l1; l1 = l1->next){
      if (l1->special || l1->region == nullptr) continue;
      if (l1->start == l1->end) continue;
      int end = l1->end;
      if (end == -1) end = line->length();
      if (l1->start > pos){
        textWriter->write(line, pos, l1->start - pos);
        pos = l1->start;
      }
      markupWriter->write(DString("<span class='"));

      const Region *region = l1->region;
      while(region != nullptr){
        SString *token0 = SString(region->getName()).replace(DString(":"),DString("-"));
        SString *token = token0->replace(DString("."),DString("-"));
        delete token0;
        markupWriter->write(token);
        delete token;
        region = region->getParent();
        if (region != nullptr){
          markupWriter->write(' ');
        }
      }

      markupWriter->write(DString("'>"));
      textWriter->write(line, pos, end - l1->start);
      markupWriter->write(DString("</span>"));
      pos += end - l1->start;
    }
    if (pos < line->length()){
      textWriter->write(line, pos, line->length() - pos);
    }
  }


  /** Write specified line of text using list of LineRegion's.
      This method uses text fields of LineRegion class to enwrap each line
      region.
      It uses two Writers - @c markupWriter and @c textWriter.
      @c markupWriter is used to write markup elements of LineRegion,
      and @c textWriter is used to write line content.
      @param markupWriter Writer, used for markup output
      @param textWriter Writer, used for text output
      @param line Line of text
      @param lineRegions Linked list of LineRegion structures
  */
  static void markupWrite(Writer *markupWriter, Writer *textWriter, std::unordered_map<SString, String*> *docLinkHash, String *line, LineRegion *lineRegions){
    int pos = 0;
    for(LineRegion *l1 = lineRegions; l1; l1 = l1->next){
      if (l1->special || l1->rdef == nullptr) continue;
      if (l1->start == l1->end) continue;
      int end = l1->end;
      if (end == -1) end = line->length();
      if (l1->start > pos){
        textWriter->write(line, pos, l1->start - pos);
        pos = l1->start;
      };
      if (l1->texted()->start_back != nullptr) markupWriter->write(l1->texted()->start_back);
      if (l1->texted()->start_text != nullptr) markupWriter->write(l1->texted()->start_text);
      textWriter->write(line, pos, end - l1->start);
      if (l1->texted()->end_text != nullptr) markupWriter->write(l1->texted()->end_text);
      if (l1->texted()->end_back != nullptr) markupWriter->write(l1->texted()->end_back);
      pos += end - l1->start;
    }
    if (pos < line->length()){
      textWriter->write(line, pos, line->length() - pos);
    }
  }


  /** Write specified line of text using list of LineRegion's.
      This method uses integer fields of LineRegion class
      to enwrap each line region with generated HTML markup.
      Each region is
      @param markupWriter Writer, used for markup output
      @param textWriter Writer, used for text output
      @param line Line of text
      @param lineRegions Linked list of LineRegion structures
  */
  static void htmlRGBWrite(Writer *markupWriter, Writer *textWriter, std::unordered_map<SString, String*> *docLinkHash, String *line, LineRegion *lineRegions){
    int pos = 0;
    for(LineRegion *l1 = lineRegions; l1; l1 = l1->next){
      if (l1->special || l1->rdef == nullptr) continue;
      if (l1->start == l1->end) continue;
      int end = l1->end;
      if (end == -1) end = line->length();
      if (l1->start > pos){
        textWriter->write(line, pos, l1->start - pos);
        pos = l1->start;
      };
      if (docLinkHash->size() > 0)
        writeHref(markupWriter, docLinkHash, l1->scheme, DString(line, pos, end - l1->start), true);
      writeStart(markupWriter, l1->styled());
      textWriter->write(line, pos, end - l1->start);
      writeEnd(markupWriter, l1->styled());
      if (docLinkHash->size() > 0)
        writeHref(markupWriter, docLinkHash, l1->scheme, DString(line, pos, end - l1->start), false);
      pos += end - l1->start;
    }
    if (pos < line->length()){
      textWriter->write(line, pos, line->length() - pos);
    }
  }

  /** Puts into stream style attributes from RegionDefine object.
  */
  static void writeStyle(Writer *writer, const StyledRegion *lr){
    static char span[256];
    int cp = 0;
    if (lr->bfore) cp += sprintf(span, "color:#%.6x; ", lr->fore);
    if (lr->bback) cp += sprintf(span+cp, "background:#%.6x; ", lr->back);
    if (lr->style&StyledRegion::RD_BOLD) cp += sprintf(span+cp, "font-weight:bold; ");
    if (lr->style&StyledRegion::RD_ITALIC) cp += sprintf(span+cp, "font-style:italic; ");
    if (lr->style&StyledRegion::RD_UNDERLINE) cp += sprintf(span+cp, "text-decoration:underline; ");
    if (lr->style&StyledRegion::RD_STRIKEOUT) cp += sprintf(span+cp, "text-decoration:strikeout; ");
    if (cp > 0) writer->write(DString(span));
  }

  /** Puts into stream starting HTML \<span> tag with requested style specification
  */
  static void writeStart(Writer *writer, const StyledRegion *lr){
    if (!lr->bfore && !lr->bback) return;
    writer->write(DString("<span style='"));
    writeStyle(writer, lr);
    writer->write(DString("'>"));
  }

  /** Puts into stream ending HTML \</span> tag
  */
  static void writeEnd(Writer *writer, const StyledRegion *lr){
    if (!lr->bfore && !lr->bback) return;
    writer->write(DString("</span>"));
  }

  static void writeHref(Writer *writer, std::unordered_map<SString, String*> *docLinkHash, const Scheme *scheme, const String &token, bool start){
    String *url = nullptr;
    if (scheme != nullptr){
      auto it_url = docLinkHash->find(&(SString(token).append(DString("--")).append(scheme->getName())));
      if (it_url != docLinkHash->end())
      {
        url = it_url->second;
      }
    }
    if (url == nullptr){
      auto it_url = docLinkHash->find(&token);
      if (it_url != docLinkHash->end())
      {
        url = it_url->second;
      }
    }
    if (url != nullptr){
      if (start) writer->write(SString("<a href='")+url+DString("'>"));
      else writer->write(DString("</a>"));
    }
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

