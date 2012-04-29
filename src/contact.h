#ifndef CONTACT_H
#define CONTACT_H

#include <string>

using namespace std;

class Contact
{
    public:
        Contact();
        virtual ~Contact();

        string getFirstname() { return m_firstname; }
        void setFirstname(string val) { m_firstname = val; }

        string getSecondname() { return m_secondname; }
        void setSecondname(string val) { m_secondname = val; }

        string getLastname() { return m_lastname; }
        void setLastname(string val) { m_lastname = val; }

        string getStreet() { return m_street; }
        void setStreet(string val) { m_street = val; }

        unsigned int getId() { return m_id; }
        void setId(unsigned int val) { m_id = val; }

        string getCity() { return m_city; }
        void setCity(string val) { m_city = val; }

        string getPostalcode() { return m_postalcode; }
        void setPostalcode(string val) { m_postalcode = val; }

        string getCountry() { return m_country; }
        void setCountry(string val) { m_country = val; }

        string getPhone() { return m_phone; }
        void setPhone(string val) { m_phone = val; }

        string getMobile() { return m_mobile; }
        void setMobile(string val) { m_mobile = val; }

        string getDayofbirth() { return m_dayofbirth; }
        void setDayofbirth(string val) { m_dayofbirth = val; }

        string getEmail() { return m_email; }
        void setEmail(string val) { m_email = val; }

        string getImage() { return m_image; }
        void setImage(string val) { m_image = val; }

    protected:
    private:
        unsigned int m_id;
        string m_firstname;
        string m_secondname;
        string m_lastname;
        string m_street;
        string m_city;
        string m_postalcode;
        string m_country;
        string m_phone;
        string m_mobile;
        string m_dayofbirth;
        string m_email;
        string m_image;
};

#endif // CONTACT_H
