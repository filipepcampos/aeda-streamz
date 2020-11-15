#include "Menu.h"
#include <sstream>

Menu::Menu(Platform &platform) : platform(platform) {}

template <typename T>
bool Menu::getInput(T &var) const {
    std::string str;
    std::cout << "> ";
    std::getline(std::cin, str);
    if(str.empty()){
        return false;
    }
    std::cout << std::endl;
    std::istringstream ss{str};
    ss >> var;
    if(ss.fail() || !ss.eof()){
        return false;
    }
    return true;
}
std::string Menu::getRawInput() const{
    std::string str;
    std::cout << "> ";
    std::getline(std::cin, str);
    std::cout << std::endl;
    return str;
}

void Menu::waitEnter() const {
    std::cout << std::endl << "Press enter to continue..." << std::endl;
    std::string str;
    std::getline(std::cin, str);
}

Menu * Menu::invalidOption() {
    std::cout << "Invalid option" << std::endl;
    waitEnter();
    return this;
}

// --------------- Main Menu ---------------

MainMenu::MainMenu(Platform &platform) : Menu(platform) {}
void MainMenu::show() {
    unsigned int options = 0;

    std::cout << CLR_SCREEN;
    cout << "\n"
            "   _____ _                            ______\n"
            "  / ____| |                          |___  /\n"
            " | (___ | |_ _ __ ___  __ _ _ __ ___    / / \n"
            "  \\___ \\| __| '__/ _ \\/ _` | '_ ` _ \\  / /  \n"
            "  ____) | |_| | |  __/ (_| | | | | | |/ /__ \n"
            " |_____/ \\__|_|  \\___|\\__,_|_| |_| |_/_____|\n"
            "\n";

    /* MAIS TARDE AJUSTAR ESPACAMENTO COM SETW() */
    cout << "Main Menu:\n\n";
    cout << "[" << ++options << "] " << "Login User\n";
    cout << "[" << ++options << "] " << "Register User\n";
    cout << "[" << ++options << "] " << "Administrator\n";
    cout << "[" << ++options << "] " << "Show information\n";
    cout << "[" << ++options << "] " << "Manual Save\n\n"; // ?
    cout << "[0] Exit\n";
}
Menu * MainMenu::getNextMenu() {
    int option; getInput<int>(option);
    switch(option){
        case 0: return nullptr;
        case 1: return new LoginUserMenu(platform);
        case 2: return new RegisterUserMenu(platform);
        case 3: return new AdministratorMenu(platform);
        case 4: return new InformationMenu(platform);
        case 5: platform.save(); return this;
    }
    return invalidOption();
}

// --------------- Register User Menu ---------------
RegisterUserMenu::RegisterUserMenu(Platform &platform) : Menu(platform) {}
void RegisterUserMenu::show() {
    std::cout << CLR_SCREEN;
    std::cout << "Information required: username, name, birthdate, type of user\n";
}
Menu * RegisterUserMenu::getNextMenu() {
    std::string username, birth_date, name, type;
    std::cout << "username\n ";
    if(!getInput<string>(username)){
        return invalidOption();
    }
    std::cout << "name\n "; name = getRawInput();
    if(name.empty()){
        return invalidOption();
    }
    std::cout << "birth date\n ";
    if(!getInput(birth_date)){
        return invalidOption();
    }
    std::cout << "type of user (viewer or streamer)\n ";
    if(!getInput(type)){
        return invalidOption();
    }
    if(type == "viewer"){
        try{
            platform.registerViewer(username, name, birth_date);
            std::cout << "Registered with success!" << std::endl;
        }
        catch(const UserAlreadyExists &e){
            std::cout << "Username already exists" << std::endl;
        }
    }
    else if(type == "streamer"){
        try{
            platform.registerStreamer(username, name, birth_date);
            std::cout << "Registered with success!" << std::endl;
        }
        catch(const UserAlreadyExists &e){
            std::cout << "Username already exists" << std::endl;
        }
    }
    waitEnter();
    return nullptr;
}

// --------------- Login User Menu ---------------
LoginUserMenu::LoginUserMenu(Platform &platform) : Menu(platform) {}
void LoginUserMenu::show() {
    if(!logged_in){
        std::cout << CLR_SCREEN;
        std::cout << "Username: ";
    }
}
Menu * LoginUserMenu::getNextMenu() {
    if(!logged_in){
        std::string username;
        if(!getInput(username)){
            return invalidOption();
        }
        try{
            User * ptr = platform.getUser(username);
            Streamer * streamer_ptr = dynamic_cast<Streamer *>(ptr);
            if(streamer_ptr){
                logged_in = true;
                return new StreamerMenu(platform, streamer_ptr);
            }
            Viewer * viewer_ptr = dynamic_cast<Viewer *>(ptr);
            if(viewer_ptr){
                logged_in = true;
                return new ViewerMenu(platform, viewer_ptr);
            }
        }
        catch(const UserDoesNotExist &e){
            std::cout << "User " << e.getNickname() << " does not exist" << endl;
            waitEnter();
        }
    }
    return nullptr;
}


// --------------- Viewer Menu ---------------

ViewerMenu::ViewerMenu(Platform &platform, Viewer *viewer) : Menu(platform), viewer(viewer) {}
void ViewerMenu::show() {
    std::cout << CLR_SCREEN;
    unsigned int option = 1;
    if(viewer->inStream()){
        std::cout << "[" << option++ << "] Leave Stream" << std::endl;
        std::cout << "[" << option++ << "] Like" << std::endl;
        std::cout << "[" << option++ << "] Dislike" << std::endl;
    }
    else{
        std::cout << "[" << option++ << "] Join Stream" << std::endl;
    }
    std::cout << "[" << option++ << "] Delete Account" << std::endl << std::endl;
    std::cout << "[0] Exit" << std::endl;
}
Menu * ViewerMenu::getNextMenu() {
    int option; getInput(option);
    if(option == 0){
        return nullptr;
    }
    if(viewer->inStream()){
        switch(option){
            case 1: viewer->leaveStream(); return this;
            case 2: std::cout << "Under development...\n"; return this;
            case 3: std::cout << "Under development...\n"; return this;
            case 4: platform.deleteUser(viewer->getNickname()); return nullptr;
        }
    }
    else{
        switch(option){
            case 1: return new JoinStreamMenu(platform, viewer);
            case 2: platform.deleteUser(viewer->getNickname()); return nullptr;
        }
    }
    return invalidOption();
}

// --------------- Streamer Menu ---------------

StreamerMenu::StreamerMenu(Platform &platform, Streamer *streamer) : Menu(platform), streamer(streamer) {}
void StreamerMenu::show() {
    unsigned int option = 1;
    std::cout << CLR_SCREEN;
    std::cout << "[" << option++ << "] Information" << std::endl;
    if(streamer->inStream()){
        std::cout << "[" << option++ << "] End Stream" << std::endl;
    }
    else{
        std::cout << "[" << option++ << "] Start stream" << std::endl;
    }
    std::cout << "[" << option++ << "] Delete account" << std::endl << std::endl;
    std::cout << "[0] Exit" << std::endl;
}

Menu * StreamerMenu::getNextMenu() {
    unsigned int option; getInput(option);
    switch (option) {
        case 1: return new InformationMenu(platform);
        case 3: platform.deleteUser(streamer->getNickname()); return nullptr;
        case 0: return nullptr;
    }
    if(option == 2){
        if(streamer->inStream()){
            streamer->endStream();
            return this;
        }
        else{
            return new CreateStreamMenu(platform, streamer);
        }
    }
    return invalidOption();
}

// --------------- Administrator Menu ---------------

AdministratorMenu::AdministratorMenu(Platform &platform) : Menu(platform), admin(platform) {}
void AdministratorMenu::show() {
    unsigned int option = 1;
    std::cout << CLR_SCREEN;
    for(const auto &str : {"Show average views", "Filter streams", "Show Top Language", "Show Top Stream Type", "Show Top Streamer"}){
        std::cout << "[" << option++ << "] " << str << std::endl;
    }
    std::cout << "[0] Exit" << std::endl;
}

Menu * AdministratorMenu::getNextMenu() {
    int option; getInput<int>(option);
    switch(option){
        case 1: std::cout << "Average views per stream: " << admin.averageViews() << std::endl; waitEnter(); return this;
        case 2: return new FilterStreamsMenu(platform, admin);
        case 3: std::cout << "Most used language in streams: " << admin.topLanguage() << std::endl; waitEnter(); return this;
        case 4: std::cout << "Most used stream type in streams: " << admin.topTypeStream() << std::endl; waitEnter(); return this;
        case 5:
            std::cout << "Streamer with most views: ";
            Streamer * strm = admin.topStreamer();
            if(strm){
                strm->show();
            }
            else{
                std::cout << "No streamer registered";
            }
            std::cout << std::endl; waitEnter(); return this;
    }
    return nullptr;
}

// --------------- Filter Streams Menu ---------------
FilterStreamsMenu::FilterStreamsMenu(Platform &platform, Admin & admin) : Menu(platform), admin(admin) {}
void FilterStreamsMenu::show() {
    std::cout << "Information required: streams type (public or private) and lower and upper time interval limit\n";
}
Menu * FilterStreamsMenu::getNextMenu() {
    std::string type, lower_date, upper_date;

    std::cout << "Stream type\n "; getInput<std::string>(type);
    while (type != "public" && type != "private") {
        std::cout << "Invalid type\n";
        getInput<std::string>(type);
    }
    /* MAIS TARDE VERIFICAR DATAS INVALIDAS */
    std::cout << "Lower date\n "; getInput<std::string>(lower_date);
    std::cout << "Upper date\n "; getInput<std::string>(upper_date);
    /* MAIS TARDE VERIFICAR DATAS INVALIDAS */
    std::cout << "Number of " << type << " streams between " << lower_date << " and " << upper_date << ": ";
    std::cout << admin.streamsCounter(type == "public", Date(lower_date), Date(upper_date)) << std::endl;
    waitEnter();
    return nullptr;
}

// --------------- Information Menu ---------------

InformationMenu::InformationMenu(Platform &platform) : Menu(platform) {}
void InformationMenu::show() {
    unsigned int option = 1;
    std::cout << CLR_SCREEN;
    for(const auto &str : {"Show top active streams", "Show Top archived streams", "Show all active streams", "Show users", "Sort active streams"}){
        std::cout << "[" << option++ << "] " << str << std::endl;
    }
    std::cout << std::endl << "[0] Exit" << std::endl;
}
Menu * InformationMenu::getNextMenu() {
    unsigned int option; getInput(option);
    switch(option){
        case 0: return nullptr;
        case 1: platform.topActiveStreams(); waitEnter(); return this;
        case 2: platform.topArchivedStreams(); waitEnter(); return this;
        case 3: platform.showStreams(); waitEnter(); return this;
        case 4: platform.showUsers(); waitEnter(); return this;
        case 5: return new SortMenu(platform);
    }
    return invalidOption();
}

// --------------- Sort Menu ---------------
SortMenu::SortMenu(Platform &platform) : Menu(platform) {}
void SortMenu::show() {
    if(!stage2){
        std::cout << CLR_SCREEN << "Sort by: " << std::endl;
        unsigned int options = 1;
        std::cout << "[" << options++ << "] Views" << std::endl;
        std::cout << "[" << options++ << "] Likes" << std::endl;
        std::cout << "[" << options++ << "] Id" << std::endl;
        std::cout << "[" << options++ << "] Minimum Age" << std::endl << std::endl;
        std::cout << "[0] Exit" << std::endl;
    }
    else{
        std::cout << CLR_SCREEN << "Sort order: " << std::endl;
        unsigned int options = 1;
        std::cout << "[" << options++ << "] Ascending" << std::endl;
        std::cout << "[" << options++ << "] Descending" << std::endl << std::endl;
        std::cout << "[" << options++ << "] Back" << std::endl;
        std::cout << "[0] Exit" << std::endl;
    }
}
Menu * SortMenu::getNextMenu() {
    static sortingMode mode;
    int option; getInput(option);
    if(!stage2){
        switch(option){
            case 0: return nullptr;
            case 1: mode = views; stage2 = true; return this;
            case 2: mode = likes; stage2 = true; return this;
            case 3: mode = id; stage2 = true; return this;
            case 4: mode = minimum_age; stage2 = true; return this;
        }
        return invalidOption();
    }
    else{
        switch(option){
            case 0: return nullptr;
            case 1: platform.sort(mode, ascending); return nullptr;
            case 2: platform.sort(mode, descending); return nullptr;
            case 3: stage2 = false; return this;
        }
        return invalidOption();
    }
}

// ------------- Create Stream Menu -----------------
CreateStreamMenu::CreateStreamMenu(Platform &platform, Streamer * streamer) : Menu(platform), streamer(streamer){}
void CreateStreamMenu::show() {
    std::cout << CLR_SCREEN << "Please fill the following parameters" << std::endl;
}
Menu * CreateStreamMenu::getNextMenu() {
    std::cout << "Stream type ('public' or 'private')" << std::endl;
    std::string type;
    if(!getInput(type)){
        return invalidOption();
    }
    if(type == "public" || type == "private"){
        std::cout << "Stream title" << std::endl;
        std::string title = getRawInput();
        if(title.empty()){
            return invalidOption();
        }
        std::cout << "Language" << std::endl;
        std::string language;
        if(!getInput(language)){
            return invalidOption();
        }
        std::cout << "Minimum age" << std::endl;
        unsigned int minimum_age;
        if(!getInput(minimum_age)){
            return invalidOption();
        }
        if(type == "public"){
            streamer->startPublicStream(title, language, minimum_age);
        }
        else{
            std::cout << "Maximum capacity" << std::endl;
            unsigned int max_capacity;
            if(!getInput(max_capacity)){
                return invalidOption();
            }

            vector<string> allowed;
            std::string user;
            std::cout << "Allowed users (Write all users you wish to allow into the stream, press enter with no username to complete" << std::endl;

            while(true){
                if(!getInput(user)){
                    break;
                }
                allowed.push_back(user);
            }
            streamer->startPrivateStream(title, language, minimum_age, max_capacity, allowed);
        }
        std::cout << "Stream created with success!" << std::endl;
        waitEnter();
        return nullptr;
    }
    return invalidOption();
}

// ------------- Join Stream Menu -----------------
JoinStreamMenu::JoinStreamMenu(Platform &platform, Viewer *viewer) : Menu(platform), viewer(viewer) {}
void JoinStreamMenu::show() {
    std::cout << CLR_SCREEN << "Join stream" << std::endl << "Stream id:" << std::endl;
}
Menu * JoinStreamMenu::getNextMenu() {
    unsigned int id; getInput(id);
    try{
        viewer->joinStream(id);
        return nullptr;
    }
    catch(const StreamDoesNotExist &e){
        std::cout << "Stream with id " << id << " does not exist" << std::endl;
        waitEnter();
        return nullptr;
    }
    catch(const StreamNoLongerActive &e){
        std::cout << "Stream with id " << id << " already ended" << std::endl;
        waitEnter();
        return nullptr;
    }
}