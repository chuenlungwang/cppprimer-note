#include <set>
#include <string>
#include <algorithm>

class Folder;

class Message {
friend class Folder;
friend void swap(Message &lhs, Message &rhs);
public:
    explicit Message(const std::string &str = ""):
        contents(str) {}
    Message(const Message &);
    Message(Message &&m);
    Message& operator=(const Message &);
    Message& operator=(Message &&rhs);
    ~Message();

    void save(Folder &);
    void remove(Folder &);
private:
    void add_to_Folders();
    void remove_from_Folders();
    void move_Folders(Message *);
private:
    std::string contents;
    std::set<Folder*> folders;
};

/////////////////////////////////////////////////////////////

class Folder {
friend void swap(Folder &lhs, Folder &rhs);
public:
    void addMsg(Message *);
    void remMsg(Message *);

    Folder(const Folder &);
    Folder& operator=(const Folder &);
    ~Folder();
private:
    void add_Messages();
    void remove_Messages();
private:
    std::set<Message*> messages;
};

//////////////////////////////////////////////////////////////

void Message::save(Folder &f)
{
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.remMsg(this);
}

void Message::add_to_Folders()
{
    for (auto f : folders)
        f->addMsg(this);
}

void Message::remove_from_Folders()
{
    for (auto f : folders)
        f->remMsg(this);
}

void Message::move_Folders(Message *m)
{
    folders = std::move(m->folders);
    for (auto f : folders) {
        f->remMsg(m);
        f->addMsg(this);
    }
    m->folders.clear();
}

Message::Message(const Message &msg):
    contents(msg.contents),
    folders(msg.folders)
{
    add_to_Folders();
}

Message::Message(Message &&msg):contents(std::move(msg.contents))
{
    move_Folders(&msg);
}

Message::~Message()
{
    remove_from_Folders();
}

Message&
Message::operator=(const Message &rhs)
{
    if (this == &rhs)
        return *this;
    remove_from_Folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders();
    return *this;
}

Message&
Message::operator=(Message &&rhs)
{
    if (this == &rhs)
        return *this;
    remove_from_Folders();
    contents = std::move(rhs.contents);
    move_Folders(&rhs);
    return *this;
}

void swap(Message &lhs, Message &rhs)
{
    lhs.remove_from_Folders();
    rhs.remove_from_Folders();
    using std::swap;
    swap(lhs.contents, rhs.contents);
    swap(lhs.folders, rhs.folders);
    lhs.add_to_Folders();
    rhs.add_to_Folders();
}

//////////////////////////////////////////////////////////////

void Folder::addMsg(Message *msg)
{
    messages.insert(msg);
}

void Folder::remMsg(Message *msg)
{
    messages.erase(msg);
}

void
Folder::remove_Messages()
{
    for (auto *m : messages)
        m->remove(*this);
}

void
Folder::add_Messages()
{
    for (auto *m : messages)
        m->save(*this);
}

Folder::~Folder()
{
    remove_Messages();
}

Folder::Folder(const Folder &f): messages(f.messages)
{
    add_Messages();
}

Folder&
Folder::operator=(const Folder &rhs)
{
    if (this == &rhs)
        return *this;
    remove_Messages();
    messages = rhs.messages;
    add_Messages();
    return *this;
}

void swap(Folder &lhs, Folder &rhs)
{
    using std::swap;
    lhs.remove_Messages();
    rhs.remove_Messages();
    swap(lhs.messages, rhs.messages);
    lhs.add_Messages();
    rhs.add_Messages();
}
