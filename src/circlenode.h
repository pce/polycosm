#ifndef CIRCLENODE_H
#define CIRCLENODE_H

#include "ofMain.h"


class CircleNode
{
    public:
        CircleNode();
        virtual ~CircleNode();

        void draw()
        {
            ofSetColor(255, 255, 255, 128);
            ofFill();
            ofSetCircleResolution(5);
            ofCircle(m_x, m_y, 20);
            ofSetColor(128, 128, 128, 128);
            ofNoFill();
            ofLine(m_originX, m_originY, m_x, m_y);
            ofCircle(m_x, m_y, 20);
        }
        void setVelocity(unsigned int val) {
            m_velocityX = val;
            m_velocityY = val;
        }
        unsigned int getVelocityX() { return m_velocityX; }
        void setVelocityX(unsigned int val) { m_velocityX = val; }
        unsigned int getVelocityY() { return m_velocityY; }
        void setVelocityY(unsigned int val) { m_velocityY = val; }

        int getX() { return m_x; }
        void setX(int val) { m_x = val; }

        int getY() { return m_y; }
        void setY(int val) { m_y = val; }

        int getZ() { return m_z; }
        void setZ(int val) { m_z = val; }

        unsigned int getOriginX() { return m_originX; }
        void setOriginX(unsigned int val) { m_originX = val; }

        unsigned int getOriginY() { return m_originY; }
        void setOriginY(unsigned int val) { m_originY = val; }

        unsigned int getSize() { return m_size; }
        void setSize(unsigned int val) { m_size = val; }
    protected:
    private:
        int m_x;
        int m_y;
        int m_z;
        unsigned int m_originX;
        unsigned int m_originY;
        unsigned int m_size;
        int m_velocityX;
        int m_velocityY;
};

#endif // CIRCLENODE_H
