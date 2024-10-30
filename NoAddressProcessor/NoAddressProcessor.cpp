// NoAddressProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>

class Memory {
private: 
    short data[1024] = {};
public:
    void write(short address, short value)
    {
        data[address] = value;
    }
    short read(short address)
    {
        return data[address];
    }
};

class Register
{
private:
    unsigned short value;

public:
    Register(short value)
    {
        this->value = value;
    }
    Register()
    {
        this->value = 0;
    }
    void incrementBy(unsigned short value)
    {
        this->value += value;
    }
    short getValue()
    {
        return this->value;
    }
    void setValue(unsigned short value)
    {
        this->value = value;
    }
};

class Stack
{
private:
    Register stack[8];
    int n = 8;
    int top = -1;
public:
    int readTop()
    {
        return top;
    }
    void setTop(int tempTop)
    {
        top = tempTop;
    }
    void push(short val)
    {
        int tempTop = readTop();
        if (tempTop >= n - 1)
            return;
        else {
            tempTop++;
            setTop(tempTop);
            stack[tempTop].setValue(val);
        }
    }
    short pop() {
        int tempTop = readTop();
        if (tempTop <= -1)
            return -1;
        else {
            tempTop--;
            setTop(tempTop);
            return stack[tempTop+1].getValue();
        }
    }
    short peek() {
        if (top <= -1)
            return -1;
        else
        {
            return stack[top].getValue();
        }
    }
};


int main()
{
    Memory dataMemory;
    Memory instMemory;
    Register pc;
    Register flags;
    Stack stackRegisters;
    Register comRegister;
    dataMemory.write(0, 4);
    dataMemory.write(1, 10);
    instMemory.write(0, 0b0000);
    instMemory.write(1, 0);
    instMemory.write(2, 0b0010);
    instMemory.write(3, 0b0000);
    instMemory.write(4, 1);
    instMemory.write(5, 0b0010);
    instMemory.write(6, 0b0011);
    while (true)
    {
        comRegister.setValue(instMemory.read(pc.getValue()));

        switch (comRegister.getValue())
        {
        case 0b0000: // PUSH
            pc.incrementBy(1);
            stackRegisters.push(instMemory.read(pc.getValue()));
            break;
        case 0b0001: // POP
            stackRegisters.pop();
            break;
        case 0b0010: // READ
            stackRegisters.push(dataMemory.read(stackRegisters.pop()));
            break;
        case 0b0011: //ADD
            stackRegisters.push(stackRegisters.pop() + stackRegisters.pop());
            break;
        case 0b0100: //SUB
            stackRegisters.push(stackRegisters.pop() - stackRegisters.pop());
            break;
        case 0b0101: //WRITE
            instMemory.write(stackRegisters.pop(), stackRegisters.pop());
            break;
        case 0b0110: //LDC
            pc.setValue(stackRegisters.pop());
            break;
        case 0b0111: // STC
            stackRegisters.push(pc.getValue());
            break;
        case 0b1010: // COM
            acc.setValue(~stackRegisters.pop());
            break;
        case 0b1011: //
            break;
        case 0b1100: 
            
            break;
        case 0b1101: // JZ
            break;
        
        case 0b1111: // OUT
            std::cout << acc.getValue();
            break;
            */
        }

        pc.incrementBy(1);
    }
}

/*case 0b1110: // INP
    short readme;
    std::cin >> readme;
    acc.setValue(readme);
    break;
    case 0b0111: //AND
            acc.setValue(acc.getValue() & stackRegisters.pop());
            break;
        case 0b1000: // OR
            acc.setValue(acc.getValue() | stackRegisters.pop());
            break;
        case 0b1001: // XOR
            acc.setValue(acc.getValue() ^ stackRegisters.pop());
            break;*/
