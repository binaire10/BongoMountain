#include "Crypto.hpp"
#include "../core/Log.hpp"
#include "../core/memory.hpp"
#include <iostream>

crypto::PKCS12_ptr crypto::loadPSCK12(std::string_view filename)
{
    return fixe_string(filename, [](auto filename){
        // Charger la clé privée RSA à partir du fichier PKCS12
        FILE *p12File = fopen(filename.data(), "rb");// Ouvrir le fichier PKCS12 en mode binaire
        if(!p12File)
        {// Vérifier si le fichier a été ouvert avec succès
            BM_CORE_ERROR("Impossible d'ouvrir le fichier PKCS12.");
            return PKCS12_ptr{};// Retourner false en cas d'erreur
        }
        PKCS12_ptr p12{ d2i_PKCS12_fp(p12File, nullptr) };// Lire le fichier PKCS12
        fclose(p12File);                                  // Fermer le fichier PKCS12
        return p12;
    });
}

crypto::EVP_PKEY_ptr crypto::getPrivateKey(const crypto::PKCS12_ptr &p12, std::string_view password)
{
    return fixe_string(password, [&p12](auto password){
        EVP_PKEY *pkey = nullptr;
        if(!PKCS12_parse(p12.get(), password.data(), &pkey, nullptr, nullptr))
        {                                  // Extraire la clé privée RSA sans mot de passe
            BM_CORE_ERROR("Impossible d'extraire la PKEY du PKCS12.");
            return EVP_PKEY_ptr{};// Retourner false en cas d'erreur
        }
        return EVP_PKEY_ptr{ pkey };
    });
}
