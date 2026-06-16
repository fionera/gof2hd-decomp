#ifndef GOF2_PARTICLESETTINGSREF_H
#define GOF2_PARTICLESETTINGSREF_H

// ParticleSettingsRef holds the global setup for the engine's particle system.
// It carries no per-instance state; initialize() seeds the two referenced
// ParticleSettings singletons and a shared counter.
class ParticleSettingsRef {
public:
    // Initialise the two referenced ParticleSettings singletons and seed the
    // shared particle counter.
    static void initialize();
};

#endif
