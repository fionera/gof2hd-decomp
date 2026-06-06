struct ApplicationManager {
    void *first;
};

struct IApplicationModule {
    void *pad0;
    void *first;
    ApplicationManager *manager;
};

extern "C" void SetApplicationManager(IApplicationModule *self, ApplicationManager *manager)
{
    self->manager = manager;
    self->first = manager->first;
}
