#ifndef GOF2_PARTICLESETTINGSREF_H
#define GOF2_PARTICLESETTINGSREF_H

class ParticleSettingsRef {
public:
    // Initialise the two referenced ParticleSettings singletons and seed the
    // shared particle counter.
    static void initialize();
};

#endif
