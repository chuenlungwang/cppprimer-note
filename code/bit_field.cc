typedef unsigned int Bit;
class File {
    Bit mode: 2;       // mode has 2 bits
    Bit modified: 1;   // modified has 1 bit
    Bit prot_owner: 3; // prot_owner has 3 bits
    Bit prot_group: 3; // prot_group has 3 bits
    Bit prot_world: 3; // prot_world has 3 bits
public:
    enum modes { READ = 01, WRITE = 02, EXECUTE = 03 };

    File &open(modes);
    void close();
    void write();
    bool isRead() const;
    void setWrite();
};

void File::write()
{
    modified = 1;
}

void File::close()
{
    if (modified)
        /* do nothing */;
}

File &File::open(File::modes m)
{
    mode |= READ;
    if (m & WRITE)
        /* do nothing */;
    return *this;
}

inline bool File::isRead() const
{
    return mode & READ;
}

inline void File::setWrite()
{
    mode |= WRITE;
}

// That is realy not good, I am feeling very bad now,
// My reMarkable paper tablet does not ship to me.
// I waited 11 days, 11 days, 11 days, not good, you know,
// I just feel I am in ice-hole. Could glory God help me?
// I hate this day.
