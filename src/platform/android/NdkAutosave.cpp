#include "platform/android/NdkHooks.h"

#include "engine/render/Touch.h"
#include "game/core/Globals.h"

#include <cstdlib>
#include <cstring>
#include <new>

// The original allocates a one-entry buffer at static-init time (its second .init_array entry:
// touches = realloc(touches, 16)).
Touch *touches;
int maxTouchSize = 0;
int curTouchSize = 0;

// The original NdkHooks TU ends at ndk_isInMainMenu (0x947e0); this function starts the next
// TU (0x947f0).  Keeping a real function here preserves that TU boundary and its .ARM.exidx
// marker.  The touch globals above are first mentioned in this TU, after NdkHooks' zip_open
// and before LODManager's ArrayRemove<AEGeometry*>, exactly as in the original .dynsym.
char *pConstToNonConst(const char *s) {
    if (s != nullptr) {
        char *copy = static_cast<char *>(std::malloc(std::strlen(s) + 1));
        if (copy != nullptr) {
            std::strcpy(copy, s);
            return copy;
        }
    }
    return nullptr;
}

const char *getStringUTFChars(JNIEnv *env, jstring str, jboolean *isCopy) {
    if (env != nullptr && str != nullptr)
        return env->GetStringUTFChars(str, isCopy);
    return nullptr;
}

void releaseStringUTFChars(JNIEnv *env, jstring str, const char *chars) {
    if (env != nullptr && str != nullptr && chars != nullptr) {
        env->ReleaseStringUTFChars(str, chars);
        return;
    }
    if (chars != nullptr) {
        operator delete(const_cast<char *>(chars));
    }
    if (str != nullptr) {
        operator delete(reinterpret_cast<void *>(str)); // lint: void_ptr NDK boundary, operator delete ABI requires void*
        // lint: void_ptr NDK boundary, operator delete ABI requires void*
    }
}

void ndk_setNativeItemInformationList(JNIEnv *env, jclass /*clazz*/,
                                      jobjectArray ids, jobjectArray names,
                                      jobjectArray descriptions,
                                      jobjectArray currencies,
                                      jobjectArray prices) {
    if (env == nullptr)
        return;

    jstring id0 = static_cast<jstring>(env->GetObjectArrayElement(ids, 0));
    jstring id1 = static_cast<jstring>(env->GetObjectArrayElement(ids, 1));
    jstring id2 = static_cast<jstring>(env->GetObjectArrayElement(ids, 2));
    jstring id3 = static_cast<jstring>(env->GetObjectArrayElement(ids, 3));
    jstring id4 = static_cast<jstring>(env->GetObjectArrayElement(ids, 4));

    jstring name0 = static_cast<jstring>(env->GetObjectArrayElement(names, 0));
    jstring name1 = static_cast<jstring>(env->GetObjectArrayElement(names, 1));
    jstring name2 = static_cast<jstring>(env->GetObjectArrayElement(names, 2));
    jstring name3 = static_cast<jstring>(env->GetObjectArrayElement(names, 3));
    jstring name4 = static_cast<jstring>(env->GetObjectArrayElement(names, 4));

    jstring description0 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 0));
    jstring description1 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 1));
    jstring description2 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 2));
    jstring description3 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 3));
    jstring description4 = static_cast<jstring>(env->GetObjectArrayElement(descriptions, 4));

    jstring currency0 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 0));
    jstring currency1 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 1));
    jstring currency2 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 2));
    jstring currency3 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 3));
    jstring currency4 = static_cast<jstring>(env->GetObjectArrayElement(currencies, 4));

    jstring price0 = static_cast<jstring>(env->GetObjectArrayElement(prices, 0));
    jstring price1 = static_cast<jstring>(env->GetObjectArrayElement(prices, 1));
    jstring price2 = static_cast<jstring>(env->GetObjectArrayElement(prices, 2));
    jstring price3 = static_cast<jstring>(env->GetObjectArrayElement(prices, 3));
    jstring price4 = static_cast<jstring>(env->GetObjectArrayElement(prices, 4));

    const char *idUtf0 = env->GetStringUTFChars(id0, nullptr);
    const char *idUtf1 = env->GetStringUTFChars(id1, nullptr);
    const char *idUtf2 = env->GetStringUTFChars(id2, nullptr);
    const char *idUtf3 = env->GetStringUTFChars(id3, nullptr);
    const char *idUtf4 = env->GetStringUTFChars(id4, nullptr);

    const char *nameUtf0 = env->GetStringUTFChars(name0, nullptr);
    const char *nameUtf1 = env->GetStringUTFChars(name1, nullptr);
    const char *nameUtf2 = env->GetStringUTFChars(name2, nullptr);
    const char *nameUtf3 = env->GetStringUTFChars(name3, nullptr);
    const char *nameUtf4 = env->GetStringUTFChars(name4, nullptr);

    const char *descriptionUtf0 = env->GetStringUTFChars(description0, nullptr);
    const char *descriptionUtf1 = env->GetStringUTFChars(description1, nullptr);
    const char *descriptionUtf2 = env->GetStringUTFChars(description2, nullptr);
    const char *descriptionUtf3 = env->GetStringUTFChars(description3, nullptr);
    const char *descriptionUtf4 = env->GetStringUTFChars(description4, nullptr);

    const char *currencyUtf0 = env->GetStringUTFChars(currency0, nullptr);
    const char *currencyUtf1 = env->GetStringUTFChars(currency1, nullptr);
    const char *currencyUtf2 = env->GetStringUTFChars(currency2, nullptr);
    const char *currencyUtf3 = env->GetStringUTFChars(currency3, nullptr);
    const char *currencyUtf4 = env->GetStringUTFChars(currency4, nullptr);

    const char *priceUtf0 = env->GetStringUTFChars(price0, nullptr);
    const char *priceUtf1 = env->GetStringUTFChars(price1, nullptr);
    const char *priceUtf2 = env->GetStringUTFChars(price2, nullptr);
    const char *priceUtf3 = env->GetStringUTFChars(price3, nullptr);
    const char *priceUtf4 = env->GetStringUTFChars(price4, nullptr);

    Globals::cItemListID_00 = pConstToNonConst(idUtf0);
    Globals::cItemListID_01 = pConstToNonConst(idUtf1);
    Globals::cItemListID_02 = pConstToNonConst(idUtf2);
    Globals::cItemListID_03 = pConstToNonConst(idUtf3);
    Globals::cItemListID_04 = pConstToNonConst(idUtf4);

    Globals::cItemListName_00 = pConstToNonConst(nameUtf0);
    Globals::cItemListName_01 = pConstToNonConst(nameUtf1);
    Globals::cItemListName_02 = pConstToNonConst(nameUtf2);
    Globals::cItemListName_03 = pConstToNonConst(nameUtf3);
    // The shipped code copies the fourth name into the fifth slot as well.
    Globals::cItemListName_04 = pConstToNonConst(nameUtf3);

    Globals::cItemListDescription_00 = pConstToNonConst(descriptionUtf0);
    Globals::cItemListDescription_01 = pConstToNonConst(descriptionUtf1);
    Globals::cItemListDescription_02 = pConstToNonConst(descriptionUtf2);
    Globals::cItemListDescription_03 = pConstToNonConst(descriptionUtf3);
    Globals::cItemListDescription_04 = pConstToNonConst(descriptionUtf4);

    Globals::cItemListCurrency_00 = pConstToNonConst(currencyUtf0);
    Globals::cItemListCurrency_01 = pConstToNonConst(currencyUtf1);
    Globals::cItemListCurrency_02 = pConstToNonConst(currencyUtf2);
    Globals::cItemListCurrency_03 = pConstToNonConst(currencyUtf3);
    Globals::cItemListCurrency_04 = pConstToNonConst(currencyUtf4);

    Globals::cItemListPrice_00 = pConstToNonConst(priceUtf0);
    Globals::cItemListPrice_01 = pConstToNonConst(priceUtf1);
    Globals::cItemListPrice_02 = pConstToNonConst(priceUtf2);
    Globals::cItemListPrice_03 = pConstToNonConst(priceUtf3);
    Globals::cItemListPrice_04 = pConstToNonConst(priceUtf4);

    releaseStringUTFChars(env, id0, idUtf0);
    releaseStringUTFChars(env, id1, idUtf1);
    releaseStringUTFChars(env, id2, idUtf2);
    releaseStringUTFChars(env, id3, idUtf3);
    releaseStringUTFChars(env, id4, idUtf4);
    releaseStringUTFChars(env, name0, nameUtf0);
    releaseStringUTFChars(env, name1, nameUtf1);
    releaseStringUTFChars(env, name2, nameUtf2);
    releaseStringUTFChars(env, name3, nameUtf3);
    releaseStringUTFChars(env, name4, nameUtf4);
    releaseStringUTFChars(env, description0, descriptionUtf0);
    releaseStringUTFChars(env, description1, descriptionUtf1);
    releaseStringUTFChars(env, description2, descriptionUtf2);
    releaseStringUTFChars(env, description3, descriptionUtf3);
    releaseStringUTFChars(env, description4, descriptionUtf4);
    releaseStringUTFChars(env, currency0, currencyUtf0);
    releaseStringUTFChars(env, currency1, currencyUtf1);
    releaseStringUTFChars(env, currency2, currencyUtf2);
    releaseStringUTFChars(env, currency3, currencyUtf3);
    releaseStringUTFChars(env, currency4, currencyUtf4);
    releaseStringUTFChars(env, price0, priceUtf0);
    releaseStringUTFChars(env, price1, priceUtf1);
    releaseStringUTFChars(env, price2, priceUtf2);
    releaseStringUTFChars(env, price3, priceUtf3);
    releaseStringUTFChars(env, price4, priceUtf4);
}

