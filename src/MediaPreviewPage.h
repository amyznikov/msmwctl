/*
 * MediaPreviewPage.h
 *
 *  Created on: Mar 29, 2015
 *      Author: amyznikov
 */

#ifndef __mediapreviewpage_h__
#define __mediapreviewpage_h__

class MediaPreviewPage
    : public WContainerWidget {
public:
  typedef WContainerWidget Base;
  MediaPreviewPage(WContainerWidget * parent = 0);
  void setMediaSource(const char * url);

private:
  //WTemplate * embed;
  WFlashObject * flash;
};


#endif /* __mediapreviewpage_h__ */
