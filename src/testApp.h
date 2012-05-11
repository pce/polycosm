#ifndef TESTAPP
#define TESTAPP

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxDate.h"
#include "ofxSQLiteHeaders.h"
#include "contact.h"
#include "circlenode.h"
#include <vector>
#include <algorithm>

#define ACTION_STEP_NEXT        1       // (0000 0001)
#define ACTION_STEP_PREV        2       // (0000 0010)
#define ACTION_ALPHABETICAL     4       // (0000 0100)
// 8       // (0000 1000)
// 16      // (0001 0000)
// 32      // (0010 0000)
// 64      // (0100 0000)


class testApp : public ofBaseApp
{
	public:
	void setup();
	void update();
	void draw();
	void exit();

	void drawBackground();
	void drawCircles();
    void setupCalendar();
    void drawCalendar();
    void updateContactView();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxUICanvas *gui;
	void guiEvent(ofxUIEventArgs &e);

    bool    drawPadding;
    bool    hasPersonChanged;
    int     actionCode;
    float   red, green, blue;
    float   *buffer;
    ofImage *img;

   	private:
		ofxSQLite* sqlite;
		ofxDate date;
		int curPersonId;
        int sumOfPersons;
        Contact* contact;
        int curPersonIdIndex;
        int m_year;
        int m_month;
        int m_day;
        int m_mouseX;
        int m_mouseY;
        bool m_isCalendarVisible;
        bool m_isShnikShnakVisible;
        // for ofTrueTypeFont - source is UTF-8
        std::string utf8ToAnsiHexLatin1(string strtext)
        {
            // TODO lookupTable - http://www.mmvisual.de/Hilfe/BinTerm/T044.htm
            // german
            if ((strtext.find("ü")>0) && (strtext.find("ü")<= strtext.length()))
                strtext.replace(strtext.find("ü"), 2, "\xFC", 1);

            if ((strtext.find("ä")>0) && (strtext.find("ä")<= strtext.length()))
                strtext.replace(strtext.find("ä"), 2, "\xE4", 1);

            if ((strtext.find("ö")>0) && (strtext.find("ö")<= strtext.length()))
                strtext.replace(strtext.find("ö"), 2, "\xF6", 1);

            if ((strtext.find("Ü")>0) && (strtext.find("Ü")<= strtext.length()))
                strtext.replace(strtext.find("Ü"), 2, "\xDC", 1);

            if ((strtext.find("Ä")>0) && (strtext.find("Ä")<= strtext.length()))
                strtext.replace(strtext.find("Ä"), 2, "\xDC", 1);

            if ((strtext.find("Ö")>0) && (strtext.find("Ö")<= strtext.length()))
                strtext.replace(strtext.find("Ö"), 2, "\xDC", 1);
            // svenska
            if ((strtext.find("å")>0) && (strtext.find("å")<= strtext.length()))
                strtext.replace(strtext.find("å"), 2, "\xE5", 1);

            if ((strtext.find("Å")>0) && (strtext.find("Å")<= strtext.length()))
                strtext.replace(strtext.find("Å"), 2, "\xC5", 1);

            // special chars
            if ((strtext.find("@")>0) && (strtext.find("@")<= strtext.length()))
                strtext.replace(strtext.find("@"), 1, "\x40", 1);


            return strtext;
        }
        /*
        std::string iso88591ToUtf8(string strtext)
        {
            unsigned char *in, *out;
            while (*in)
                if (*in<128) *out++=*in++;
                else *out++=0xc2+(*in>0xbf), *out++=(*in++&0x3f)+0x80;
        }
        */

};

#endif
