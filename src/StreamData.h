#ifndef STREAMZ_STREAMDATA_H
#define STREAMZ_STREAMDATA_H
#include <iostream>
#include "Date.h"
#include <string>

using namespace std;


class StreamData {
protected:
    /**
     * Identification of the stream
     */
    unsigned id;
    /**
     * Title of the stream
     */
    string title;
    /**
     * Streamer
     */
    string streamer;
    /**
     * Start date of the stream
     */
    Date start_date;
    /**
     * End date of the stream
     */
    Date end_date;
    /**
     * Language of the stream
     */
    string language;
    /**
     * Number of viewers
     */
    unsigned viewers;
    /**
     * Accessibility of the stream
     */
    bool isPublic;
public:
    /**
     * Class constructor
     */
    StreamData(const unsigned &id, const string &title, const string &streamer,const Date &start_date, const Date &end_date,  const string &language, const unsigned &viewers, const bool &isPublic);
    /**
     * Gets the id of the stream
     * @return id of the stream
     */
    unsigned getId() const;
    /**
     * Gets the title of the stream
     * @return title of the stream
     */
    string getTitle() const;
    /**
     * Gets the streamer
     * @return streamer
     */
    string getStreamer() const;
    /**
     * Gets the start date of the stream
     * @return start date of the stream
     */
    Date getStartDate() const;
    /**
     * Gets the end date of the stream
     * @return end date of the stream
     */
    Date getEndDate() const;
    /**
     * Gets the language of the stream
     * @return language of the stream
     */
    string getLanguage() const;
    /**
     * Gets the number of viewers at the stream
     * @return number of viewers at the stream
     */
    unsigned getViewers() const;
    /**
     * Checks if stream is public or private
     * @return boolean indicating if the stream is public
     */
    bool getIsPublic() const;
    /**
     * Prints data about the stream
     * @param os Output stream where the information will be writen
     * @return Stream where the information will be writen
     */
    ostream& operator<<(ostream& os) const;
};


#endif //STREAMZ_STREAMDATA_H
