struct String;

extern "C" void OpenWrite_tail(String *, unsigned int, unsigned int *);

extern "C" void OpenWrite(String *path, unsigned int *size)
{
    return OpenWrite_tail(path, 0, size);
}
