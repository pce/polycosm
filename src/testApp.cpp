#include "testApp.h"

#define _LOCAL_
#define _DEBUG_

vector<int> personIds;
vector<CircleNode*> circleNodes;
vector<string> actionParams;

//--------------------------------------------------------------
void testApp::setup()
{
#ifdef _DEBUG_
    ofSetLogLevel(OF_LOG_VERBOSE);
#endif
    m_isShnikShnakVisible=true;
    m_isCalendarVisible=false;
	ofSetVerticalSync(true);
	ofEnableSmoothing();
  	curPersonIdIndex = 0;
    red = 32; blue = 216; green = 52;
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    drawPadding = false;
    gui = new ofxUICanvas(0,0,length+xInit,ofGetHeight());
    gui->setFont("GUI/mplus-1mn-medium.ttf");
    // gui->setFontSize(OFX_UI_FONT_LARGE, 12);
    // gui->setFontSize(OFX_UI_FONT_MEDIUM, 10);
    // gui->setFontSize(OFX_UI_FONT_SMALL, 8);

#ifdef _LOCAL_
    sqlite = new ofxSQLite(ofToDataPath("contacts.db"));
#else
    sqlite = new ofxSQLite(ofToDataPath("polycosm.db"));
#endif
	// Get default Contact.
	//-------------------------------------------------------------------------
    contact = new Contact;
	ofxSQLiteSelect sel = sqlite->select("count(person_id) as total").from("person");
	sel.execute().begin();
	sumOfPersons = sel.getInt();
	ofLogVerbose() << "total entries:" << sumOfPersons << endl;
	if (sumOfPersons >= 1) {
        // save ids
        sel = sqlite->select("person_id").from("person");
        sel.execute().begin();
        while(sel.hasNext()) {
            int id = sel.getInt();
            personIds.push_back(id);
            sel.next();
        }
        curPersonId = personIds.front();
        ofLogVerbose() << "curPersonId:" << curPersonId << std::endl;
        sel = sqlite->select("*").from("person").where("person_id", curPersonId)
                .execute()
                .begin();
        ofLogVerbose() << sel.getLiteralQuery();
        while(sel.hasNext()) {
            int id = sel.getInt();
            curPersonId = id;
            std::string firstname = sel.getString();
            // http://htmlhelp.com/de/reference/html40/entities/latin1.html
            // firstname = utf8ToAnsiHexLatin1("Rühl");
            ofLogVerbose() <<  "firstname.length:" << firstname.length() << endl;
            contact->setFirstname(utf8ToAnsiHexLatin1(firstname));
            std::string secondname = sel.getString();
            contact->setSecondname(utf8ToAnsiHexLatin1(secondname));
            std::string lastname = sel.getString();
            contact->setLastname(utf8ToAnsiHexLatin1(lastname));
            std::string street = sel.getString();
            contact->setStreet(utf8ToAnsiHexLatin1(street));
            std::string city = sel.getString();
            contact->setCity(utf8ToAnsiHexLatin1(city));
            std::string postalcode= sel.getString();
            contact->setPostalcode(utf8ToAnsiHexLatin1(postalcode));
            std::string country = sel.getString();
            contact->setCountry(utf8ToAnsiHexLatin1(country));
            std::string phone = sel.getString();
            contact->setPhone(phone);
            std::string mobile = sel.getString();
            contact->setMobile(mobile);
            std::string dayofbirth = sel.getString();
            if (dayofbirth.length())
               contact->setDayofbirth(dayofbirth);
            else
                contact->setDayofbirth("0000-00-00");
            std::string email = sel.getString();
            contact->setEmail(utf8ToAnsiHexLatin1(email));
            ofLogVerbose() << id << ", " << firstname << " " << lastname << endl;
            break;
        }
        // init circles
        int originX; int originY;
        int x; int y;

        originX = ofGetWidth()/2;
        originY = ofGetHeight()/2;
        uint8_t nodeSum = 8;
        for (int i=0; i < nodeSum; i++) {
            CircleNode* circlenode = new CircleNode;
            circlenode->setX(ofRandom(0,ofGetWidth()));
            circlenode->setY(ofRandom(0,ofGetHeight()));
            circlenode->setOriginX(originX);
            circlenode->setOriginY(originY);
            circlenode->setVelocityX(ofRandom(1,4));
            circlenode->setVelocityY(ofRandom(1,4));
            circleNodes.push_back(circlenode);
        }
	}

	gui->addWidgetDown(new ofxUILabel("CONTACTS", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));

    gui->addWidgetDown(new ofxUILabelButton(false, "<", OFX_UI_FONT_MEDIUM));
    gui->addWidgetRight(new ofxUILabelButton(false, ">", OFX_UI_FONT_MEDIUM));


    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_NAME", contact->getFirstname(), OFX_UI_FONT_MEDIUM));
    ofxUITextInput* textinput = (ofxUITextInput *) gui->getWidget("CT_NAME");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_LASTNAME", contact->getLastname(), OFX_UI_FONT_MEDIUM));
    textinput = (ofxUITextInput *) gui->getWidget("CT_LASTNAME");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_PHONE", contact->getPhone(), OFX_UI_FONT_MEDIUM));
    textinput = (ofxUITextInput *) gui->getWidget("CT_PHONE");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_MOBILE", contact->getMobile(), OFX_UI_FONT_MEDIUM));
    textinput = (ofxUITextInput *) gui->getWidget("CT_MOBILE");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_EMAIL", contact->getEmail(), OFX_UI_FONT_MEDIUM));
    textinput = (ofxUITextInput *) gui->getWidget("CT_EMAIL");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUITextInput(length-xInit, "CT_DAYOFBIRTH", contact->getDayofbirth(), OFX_UI_FONT_MEDIUM));
    textinput = (ofxUITextInput *) gui->getWidget("CT_DAYOFBIRTH");
    textinput->setAutoClear(false);

    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));

    img = new ofImage();
    // <email>|<id>.jpg
    ofFile file = ofFile("img/" + ofToString(curPersonId) + ".jpg");
    if (file.exists())
        img->loadImage("img/" + ofToString(curPersonId) + ".jpg");
    else
        img->loadImage("dummy.jpg");

    // gui->addWidgetDown(new ofxUILabel("IMAGE DISPLAY", OFX_UI_FONT_MEDIUM));
    float imgDim = (length-xInit)*.5;
    imgDim -=xInit;
    gui->addWidgetDown(new ofxUIImage(imgDim, imgDim, img, "CT_IMAGE", false));

    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));

    vector<string> items;
    items.push_back("A-D");
    items.push_back("E-H");
    items.push_back("I-L");
    items.push_back("M-O");
    items.push_back("P-R");
    items.push_back("S-U");
    items.push_back("V-Z");

    /*
    items.push_back("Jan");
    items.push_back("Feb");
    items.push_back("Mar");
    items.push_back("Apr");
    items.push_back("May");
    items.push_back("Jun");
    items.push_back("Jul");
    items.push_back("Aug");
    items.push_back("Sep");
    items.push_back("Oct");
    items.push_back("Nov");
    items.push_back("Dec");
    */
    gui->addWidgetDown(new ofxUIDropDownList("A-Z", items, OFX_UI_FONT_SMALL));
    ofxUIDropDownList* ddList = (ofxUIDropDownList *) gui->getWidget("A-Z");
    ddList->setAutoClose(true);
    ddList->setAllowMultiple(false);

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    setupCalendar();
}

//--------------------------------------------------------------

void testApp::updateContactView()
{
    ofxUITextInput* textinput = (ofxUITextInput *) gui->getWidget("CT_NAME");
    textinput->setTextString(contact->getFirstname());

    textinput = (ofxUITextInput *) gui->getWidget("CT_LASTNAME");
    textinput->setTextString(contact->getLastname());

    textinput = (ofxUITextInput *) gui->getWidget("CT_PHONE");
    textinput->setTextString(contact->getPhone());

    textinput = (ofxUITextInput *) gui->getWidget("CT_MOBILE");
    textinput->setTextString(contact->getMobile());

    textinput = (ofxUITextInput *) gui->getWidget("CT_EMAIL");
    textinput->setTextString(contact->getEmail());

    textinput = (ofxUITextInput *) gui->getWidget("CT_DAYOFBIRTH");
    textinput->setTextString(contact->getDayofbirth());

    textinput = (ofxUITextInput *) gui->getWidget("CT_DAYOFBIRTH");
    // <email>|<id>.jpg
    img = new ofImage();
    ofFile file = ofFile("img/" + ofToString(curPersonId) + ".jpg");
    if (file.exists()) {
        img->loadImage("img/" + ofToString(curPersonId) + ".jpg");
    } else
        img->loadImage("dummy.jpg");

    ofxUIImage* imgwidget =  (ofxUIImage*) gui->getWidget("CT_IMAGE");
    imgwidget->setImage(img);

}


//--------------------------------------------------------------
void testApp::update()
{
	//--person data
	if (hasPersonChanged) {
        ofLogVerbose() << "hasPersonChanged" << endl;
        hasPersonChanged = false;
       	if (sumOfPersons >= 1) {

            if (actionCode & ACTION_STEP_NEXT) {
                if (curPersonIdIndex == personIds.size()-1)
                    curPersonIdIndex = 0;
                else
                    curPersonIdIndex++;
            } else if (actionCode & ACTION_STEP_PREV) {
                if (curPersonIdIndex == 0)
                    curPersonIdIndex = personIds.size()-1;
                else
                    curPersonIdIndex--;
            }
            curPersonId =  personIds.at(curPersonIdIndex);

            ofLogVerbose() << "sumOfPersons " << sumOfPersons << ", curPersonId "<< curPersonId << endl;

            ofxSQLiteSelect sel = sqlite->select("*").from("person").where("person_id",curPersonId);

            if (actionCode & ACTION_ALPHABETICAL) {
                ofLogVerbose() << "ACTION_ALPHABETICAL" << endl;
                if (actionParams.size())
                    sel = sqlite->select("*").from("person").where("lastname", actionParams[0], WHERE_LIKE);
            }
            sel.execute().begin();
            // ofLogVerbose() << sel.getLiteralQuery() << endl;
            while(sel.hasNext()) {
                int id = sel.getInt();
                curPersonId = id;
                std::string firstname = sel.getString();
                // http://htmlhelp.com/de/reference/html40/entities/latin1.html
                // firstname = utf8ToAnsiHexLatin1("Rühl");
                ofLogVerbose() <<  "firstname.length:" << firstname.length() << endl;
                contact->setFirstname(utf8ToAnsiHexLatin1(firstname));
                std::string secondname = sel.getString();
                contact->setSecondname(utf8ToAnsiHexLatin1(secondname));
                std::string lastname = sel.getString();
                contact->setLastname(utf8ToAnsiHexLatin1(lastname));
                std::string street = sel.getString();
                contact->setStreet(utf8ToAnsiHexLatin1(street));
                std::string city = sel.getString();
                contact->setCity(utf8ToAnsiHexLatin1(city));
                std::string postalcode= sel.getString();
                contact->setPostalcode(utf8ToAnsiHexLatin1(postalcode));
                std::string country = sel.getString();
                contact->setCountry(utf8ToAnsiHexLatin1(country));
                std::string phone = sel.getString();
                contact->setPhone(phone);
                std::string mobile = sel.getString();
                contact->setMobile(mobile);
                std::string dayofbirth = sel.getString();
                if (dayofbirth.length())
                   contact->setDayofbirth(dayofbirth);
                else
                    contact->setDayofbirth("0000-00-00");
                std::string email = sel.getString();
                contact->setEmail(utf8ToAnsiHexLatin1(email));
                ofLogVerbose() << id << ", " << firstname << " " << lastname << endl;
                // sel.next();
                break;
            }
            updateContactView();
        }
	} // hasPersonChanged
	// set Date back to inital state
    date.setMonth(m_month);
    date.setYear(m_year);
    date.setDay(m_day);
    // draw nodes
	int x;
	int y;
    for (int i=0; i < circleNodes.size(); i++) {
        x = circleNodes[i]->getX();
        y = circleNodes[i]->getY();
        x += circleNodes[i]->getVelocityX();
        y += circleNodes[i]->getVelocityY();
        if (x > ofGetWidth() || x < 0) {
            circleNodes[i]->setVelocityX(circleNodes[i]->getVelocityX() * -1);
        }
        if (y > ofGetHeight() || y < 0) {
            circleNodes[i]->setVelocityY(circleNodes[i]->getVelocityY() * -1);
        }
        circleNodes[i]->setX(x);
        circleNodes[i]->setY(y);
    }
}

//--------------------------------------------------------------


void testApp::drawBackground()
{
    int height = ofGetHeight();
    int width = ofGetWidth();

    glBegin(GL_QUAD_STRIP);

        ofSetColor(red/4, blue/4, blue/4);

        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);

        ofSetColor(red, green, blue);

        glVertex3f(0, height/2, 0);
        glVertex3f(width, height/2, 0);

        ofSetColor(red, green, blue);

        glVertex3f(0, height/2, 0);
        glVertex3f(width, height/2, 0);

        ofSetColor(red/8, blue/8, blue/8);

        glVertex3f(0, height, 0);
        glVertex3f(width, height, 0);

    glEnd();

}


void testApp::setupCalendar()
{
    ofxDate date = ofxDate();
    m_month = date.getMonth();
    m_day = date.getDay();
    m_year = date.getYear();
}

void testApp::drawCalendar()
{
    // date.inspect();
    ofSetCircleResolution(5);
    ofNoFill();

    int calweek = date.getCalendarWeek();
    int dayX = 0;
    int dayY = 96;
    int dayR = 48;
    int dayH = 64;
    date.setDay(1);
    int dayNum = 0;
    ofSetHexColor(0x4488FF);
    ofDrawBitmapString(ofToString(m_year)+"-"+ofToString(m_month)+"-"+ofToString(m_day), 320 + (6 * (dayR*2)), 16);
    date = date-date.getWeekday();
    do {
        ofSetHexColor(0x4466FF);
        if (date.getMonth() == 1) calweek=1;
        ofDrawBitmapString(ofToString(calweek), 328, dayY);
        for (dayNum=0; dayNum < 7; dayNum++, date=date+1) {
            if (date.getMonth() == m_month) {
                ofSetHexColor(0xFF4488);
            } else {
                ofSetHexColor(0x5599FF);
            }
            dayX = 300 + (dayNum * (dayR*2)) + (dayR*2);
            if (date.getDay() == m_day) ofSetHexColor(0xEEFFFF);
            ofCircle(dayX, dayY, dayR);
            ofSetHexColor(0xDDEEFF);
            ofDrawBitmapString(ofToString(date.getDay()), dayX-(dayR/2), dayY);
            // ofDrawBitmapString(ofToString(date.getMonth())+"."+ofToString(date.getYear()), dayX-(dayR/2), dayY);
        }
        dayY += dayH;
        calweek++;
   }  while (date.getMonth() == m_month);

}



void testApp::drawCircles()
{
    for (int i=0; i < circleNodes.size(); i++)
        circleNodes[i]->draw();
}

void testApp::draw()
{
	drawBackground();
    if (m_isShnikShnakVisible) drawCircles();
    if (m_isCalendarVisible) drawCalendar();
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    actionCode = 0;

	if(name == "CT_NAME") {
        ofxUITextInput* textinput = (ofxUITextInput *) e.widget;
        ofLogVerbose() << textinput->getTextString() << endl;
	}

	if(kind == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        ofLogVerbose() << name << "\t value: " << button->getValue() << endl;
        // on release
        if (button->getValue() == 0) {
            if (name == "<") {
                hasPersonChanged = true;
                actionCode |= ACTION_STEP_PREV;
            } else if (name == ">") {
                hasPersonChanged = true;
                actionCode |= ACTION_STEP_NEXT;
            }
        }
    }
    // DropDownList ACTION_ALPHABETICAL
    // TODO get Items ~ vector<ofxUILabelToggle *> toggles = dd->getToggles();
    vector<string> items;
    items.push_back("A-D");
    items.push_back("E-H");
    items.push_back("I-L");
    items.push_back("M-O");
    items.push_back("P-R");
    items.push_back("S-U");
    items.push_back("V-Z");
    for (int i=0; i < items.size(); i++) {
        if(e.widget->getName() == items.at(i)) {
            hasPersonChanged = true;
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            ofLogVerbose() << "Name  : " << e.widget->getName() << " = " << toggle->getValue() << endl;
            actionCode |= ACTION_ALPHABETICAL;
            actionParams.clear();
            actionParams.push_back(items.at(i).substr(0,1) + "%");
        }
    }

}

//--------------------------------------------------------------
void testApp::exit()
{
    delete contact;
    delete sqlite;
    delete gui;
	delete[] buffer;
    delete img;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(gui->hasKeyboardFocus())
    {
        return;
    }

    switch (key)
    {
        case 'c':
            m_isCalendarVisible = !m_isCalendarVisible;
        break;
        case 'e':
            m_isShnikShnakVisible = !m_isShnikShnakVisible;
        break;
        case 'f':
            ofToggleFullscreen();
        break;
        case 's':
            ofSaveScreen("screenshot.png");
        case 'p':
            drawPadding = !drawPadding;
            gui->setDrawPadding(drawPadding);
        break;
        case 'g':
            gui->toggleVisible();
        break;
        default:
        break;
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{


}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
