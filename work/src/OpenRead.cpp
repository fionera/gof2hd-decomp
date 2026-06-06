extern "C" void OpenRead_tail(void *, int, unsigned int *);

extern "C" void OpenRead(void *path, unsigned int *out_size)
{
    return OpenRead_tail(path, 1, out_size);
}
