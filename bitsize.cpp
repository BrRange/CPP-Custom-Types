#include <iostream>

#define debug(x) std::cout << #x << " => " << x

#define iota(x) for(size_t iota(0); iota < x; iota++)

struct bitsize;
std::ostream &operator<<(std::ostream &o, bitsize bz);

struct bitsize{
    size_t bytes;
    unsigned char *data;
    bitsize(const bitsize &bz) : bytes(bz.bytes), data(new unsigned char[bz.bytes]) {
        memcpy(data, bz.data, bytes);
    }
    template<typename...Vargs>
    bitsize(Vargs...varg) : bytes(sizeof...(varg)), data(new unsigned char[sizeof...(varg)]) {
        unsigned char expansion[] = {static_cast<unsigned char>(varg)...};
        memcpy(data, &expansion, bytes);
    }
    ~bitsize(){
        delete[] data;
    }
    void resize(size_t size){
        delete[] data;
        data = new unsigned char[size];
        memset(data, 0, size);
        bytes = size;
    }
    std::string hex(size_t index){
        std::string h;
        h.resize(2);
        h[0] = '0' + (data[index] >> 4);
        if(h[0] > '9') h[0] += '\7';
        h[1] = '0' + (data[index] & 0xf);
        if(h[1] > '9') h[1] += '\7';
        return h;
    }
    bool getBit(size_t index) const{
        return data[index / 8] & (1 << (index % 8));
    }
    unsigned char getByte(size_t index) const{
        return data[index];
    }
    void setBit(size_t index, bool val){
        val ? data[index / 8] |= (1 << (index % 8)) : data[index / 8] &= ~(1 << (index % 8));
    }
    void setByte(size_t index, unsigned char val){
        data[index] = val;
    }
    void trim(){
        size_t index = bytes;
        while(index){
            index--;
            if(getByte(index)) break;
        }
        index++;
        if(index == bytes) return;
        unsigned char *newData = new unsigned char[index];
        memcpy(newData, data, index);
        delete[] data;
        data = newData;
        bytes = index;
    }
    void operator=(bitsize &&bz){
        delete[] data;
        bytes = bz.bytes;
        data = bz.data;
        bz.data = nullptr;
    }
    void operator=(bitsize &bz){
        delete[] data;
        bytes = bz.bytes;
        data = bz.data;
        bz.data = nullptr;
    }
    bitsize operator+(const bitsize &bz) const{
        short rest = 0;
        bitsize ret(0);
        ret.resize(std::max(bytes, bz.bytes) + 1);
        iota(ret.bytes){
            if(iota < bytes) rest += getByte(iota);
            if(iota < bz.bytes) rest += bz.getByte(iota);
            ret.data[iota] = rest & 0xff;
            rest >>= 8;
        }
        ret.trim();
        return ret;
    }
    bitsize operator-(const bitsize &bz) const {
        bitsize ret(0);
        ret.resize(std::max(bytes, bz.bytes) + 1);
        int borrow = 0;
        iota(ret.bytes){
            int lhs = (iota < bytes) ? getByte(iota) : 0;
            int rhs = (iota < bz.bytes) ? bz.getByte(iota) : 0;
            int diff = lhs - rhs - borrow;
            if (diff < 0) {
                diff += 256;
                borrow = 1;
            } else {
                borrow = 0;
            }
            ret.setByte(iota, diff);
        }
        ret.trim();
        return ret;
    }
    bitsize operator*(const bitsize& bz) const {
        bitsize ret(0);
        ret.resize(bytes + bz.bytes);
        iota(bz.bytes){
            unsigned char multiplier = bz.getByte(iota);
            size_t carry = 0;
            for (size_t j = 0; j < bytes; ++j) {
                size_t index = iota + j;
                size_t product = static_cast<size_t>(getByte(j)) * multiplier + ret.getByte(index) + carry;
                ret.setByte(index, product & 0xFF);
                carry = product >> 8;
            }
            if (carry != 0 && (iota + bytes) < bytes + bz.bytes) {
                ret.setByte(iota + bytes, ret.getByte(iota + bytes) + static_cast<unsigned char>(carry));
            }
        }
        ret.trim();
        return ret;
    }
    bool isEmpty(){
        iota(bytes) if(getByte(iota)) return false;
        return true;
    }
};

class pseudo{
    bitsize list;
    size_t index;
public:
    pseudo(bitsize &bz) : list(0), index(0){
        list.bytes = bz.bytes;
        list.data = bz.data;
        bz.bytes = 0;
        bz.data = nullptr;
    }
    template<typename type>
    type get(){
        type ret = 0;
        iota(sizeof(type)){
            if(index >= list.bytes) index = 0;
            ret |= static_cast<unsigned long long>(list.getByte(index)) << (iota * 8);
            index++;
        }
        return ret;
    }
    void advance(size_t amount){
        index += amount;
    }
    float rangef(){
        return get<unsigned>() / static_cast<float>(-1u);
    }
    float range(){
        return get<unsigned long long>() / static_cast<double>(-1ull);
    }
};

std::ostream &operator<<(std::ostream &o, bitsize bz){
    if(bz.bytes == 0) return o;
    for(size_t s(bz.bytes - 1); s > 0; s--) o << bz.hex(s);
    return o << bz.hex(0);
}

bitsize fact(bitsize x){
    bitsize res(1);
    while(!x.isEmpty()){
        res = res * x;
        x = x - bitsize(1);
    }
    return res;
}

bitsize fibo(bitsize bz){
    if(bz.bytes == 1 && bz.getByte(0) <= 1) return bitsize(1);
    bitsize aux1(1), aux2(1), res(0);
    do{
        res = aux1 + aux2;
        aux1 = aux2;
        aux2 = res;
        bz = bz - bitsize(1);
    } while(bz.bytes > 1 || bz.getByte(0) > 2);
    return aux2;
}

int main(){
    bitsize a(10);
    a = fact(fibo(a));
    debug(a);
}