#ifndef STREAMZ_VIEWER_H
#define STREAMZ_VIEWER_H
#include "User.h"
#include "Stream.h"
#include <memory>

class Viewer : public User {
public:
    /**
     * Class constructor
     * @param nickname Nickname of the user
     * @param name Name of the user
     * @param birth_date Birth date of the user
     * @throws InvalidAge
     */
    Viewer(const std::string &nickname, const std::string &name, const Date &birth_date, Platform &platform);
    /**
     * Gets the weak pointer to the stream the viewer is currently watching
     * @return Weak pointer to the stream the viewer is currently watching
     */
    std::weak_ptr<Stream> getCurrentStream() const;
    /**
     * Joins the stream given as an argument
     * @param stream Shared pointer to the stream the viewer will join
     */
    void joinStream(const unsigned int id);
    /**
     * Leaves the stream that is currently being watched
     */
    void leaveStream();
    /**
     * Leave a comment in the current stream (if viewer is not in private stream nothing will happen)
     * @param str - Comment content
     */
    void comment(const std::string &str);
    /**
     * Displays on the screen the info about the user
     */
    void show() const;
    /**
     * Writes in the indicated ostream the information about the user
     * @param os Stream where the information will be writen
     * @return Stream where the information will be writen
     */
    std::ostream& print(std::ostream & os) const;
    /**
     * Checks if the users have the same nickname
     * @param other Viewer to be compared with
     * @return Boolean indicating if the viewers are the same
     */
    bool operator==(const Viewer &other) const;
};

#endif //STREAMZ_VIEWER_H
