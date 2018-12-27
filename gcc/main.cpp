#include <iostream>
/*
 * https://stackoverflow.com/questions/14737104/what-is-the-default-c-mode-for-the-current-gcc-especially-on-ubuntu/53063656#53063656
 */
int main(void) {
#ifdef __cplusplus
    std::cout << __cplusplus << std::endl;
#endif
#ifdef __STRICT_ANSI__
    std::cout << "__STRICT_ANSI__" << std::endl;
#endif
    return 0;
}
