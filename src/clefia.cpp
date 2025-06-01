#include "clefia.hpp"

namespace clefia {

CipherParameters get_params(KeyLength length){
    switch(length){
        case KeyLength::Bits128:
            return {length, 18, 36};
        case KeyLength::Bits192:
            return {length, 22, 44};
        case KeyLength::Bits256:
            return {length, 26, 52};
        default:
            return {KeyLength::Bits128, 18, 36};
    }
}

}
