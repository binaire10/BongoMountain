#ifndef BONGOMOUNTAIN_GLAD_LOADER_HPP
#define BONGOMOUNTAIN_GLAD_LOADER_HPP

#include <string_view>
#include <glad/glad.h>

class GLADLayer
{
public:
    template<typename Loader>
    void initialize(Loader loader) {
        initialize(reinterpret_cast<GLADloadproc>(loader));
    }

    void initialize(GLADloadproc loader);

private:
    bool isInitialized = false;
};


#endif// BONGOMOUNTAIN_GLAD_LOADER_HPP
