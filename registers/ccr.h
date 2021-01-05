#ifndef _CCR_INCLUDED_
#define _CCR_INCLUDED_

class CCR {
private:
    unsigned char ccr;

public:
    CCR() : ccr(0) {}
    const unsigned char raw() const { return ccr; }

    bool i()  { return (this->ccr & 0x80) != 0; }
    bool ui() { return (this->ccr & 0x40) != 0; }
    bool h()  { return (this->ccr & 0x20) != 0; }
    bool u()  { return (this->ccr & 0x10) != 0; }
    bool n()  { return (this->ccr & 0x08) != 0; }
    bool z()  { return (this->ccr & 0x04) != 0; }
    bool v()  { return (this->ccr & 0x02) != 0; }
    bool c()  { return (this->ccr & 0x01) != 0; }
    
    void set_i()  { this->ccr = this->ccr | 0x80; }
    void set_ui() { this->ccr = this->ccr | 0x40; }
    void set_h()  { this->ccr = this->ccr | 0x20; }
    void set_u()  { this->ccr = this->ccr | 0x10; }
    void set_n()  { this->ccr = this->ccr | 0x08; }
    void set_z()  { this->ccr = this->ccr | 0x04; }
    void set_v()  { this->ccr = this->ccr | 0x02; }
    void set_c()  { this->ccr = this->ccr | 0x01; }

    void clear_i()  { this->ccr = this->ccr & ~0x80; }
    void clear_ui() { this->ccr = this->ccr & ~0x40; }
    void clear_h()  { this->ccr = this->ccr & ~0x20; }
    void clear_u()  { this->ccr = this->ccr & ~0x10; }
    void clear_n()  { this->ccr = this->ccr & ~0x08; }
    void clear_z()  { this->ccr = this->ccr & ~0x04; }
    void clear_v()  { this->ccr = this->ccr & ~0x02; }
    void clear_c()  { this->ccr = this->ccr & ~0x01; }

};

#endif
