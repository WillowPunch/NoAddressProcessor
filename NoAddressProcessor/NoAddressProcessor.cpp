// NoAddressProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

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
    void ror()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 2].getValue());
        stack[top - 2].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(tempVar);
        
    }
    void rol()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(stack[top - 2].getValue());
        stack[top - 2].setValue(tempVar);
    }
    void swap()
    {
        short tempVar = stack[top].getValue();
        stack[top].setValue(stack[top - 1].getValue());
        stack[top - 1].setValue(tempVar);
    }
    bool compare()
    {
        return stack[top].getValue() == stack[top - 1].getValue();
    }
    void increment(short value)
    {
        stack[top].incrementBy(value);
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
    bool noStop = true;
    dataMemory.write(10, 5);
    dataMemory.write(11, 4);
    dataMemory.write(12, 3);
    dataMemory.write(13, 2);
    dataMemory.write(14, 1);
    dataMemory.write(15, 0);
    instMemory.write(0, 0b0000); //PUSH 6 - THE SIZE OF ARRAY
    instMemory.write(1, 6);
    instMemory.write(2, 0b0000); //PUSH 10 - THE ADDRESS OF FIRST ELEMENT OF ARRAY
    instMemory.write(3, 10);
    instMemory.write(4, 0b1100); // DUP 10
    instMemory.write(5, 0b1010); // ROL
    instMemory.write(6, 0b0011); // ADD TO FIND THE END OF ARRAY
    instMemory.write(7, 0b1000); // SWAP TO PUT ADDRESS 10 ON TOP
    instMemory.write(8, 0b1100); // DUP 10 TO HAVE BOTH ADDRESS AND POINTER
    instMemory.write(9, 0b0010); // READ ADDRESS 10
    instMemory.write(10, 0b1010); // ROL TO PUT POINTER ON TOP AND ADDRESS OF LAST PRE-TOP
    instMemory.write(11, 0b1110); // INC TO NEXT ADDRESS
    instMemory.write(12, 0b1011); // CMP NEXT AND LAST ADDRESSES
    instMemory.write(13, 0b1101); // IF TRUE
    instMemory.write(14, 24); // JZ TO OUTPUT
    instMemory.write(15, 0b1001); // ROR AND SWP TO PUT NEXT ADDRESS ON TOP, SUM ON PRE-TOP
    instMemory.write(16, 0b1000);
    instMemory.write(17, 0b1100); // DUP NEXT ADDRESS
    instMemory.write(18, 0b0010); // READ ADDRESS
    instMemory.write(19, 0b1001); // ROR TO PUT TWO VALUES ON TOP
    instMemory.write(20, 0b0011); // ADD NUMBERS
    instMemory.write(21, 0b0000); // PUSH THE INSTRUCTION 10 IN STACK
    instMemory.write(22, 10);
    instMemory.write(23, 0b0110); // LDC. COUNTER IS SET TO 10. LOOP
    instMemory.write(24, 0b1001); // ROR TO PUT SUM ON TOP
    instMemory.write(25, 0b1111); // OUTPUT SUM
    instMemory.write(26, 0b0111); // FULL STOP

    while (noStop)
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
        case 0b0011: // ADD
            stackRegisters.push(stackRegisters.pop() + stackRegisters.pop());
            break;
        case 0b0100: // SUB
            stackRegisters.push(stackRegisters.pop() - stackRegisters.pop());
            break;
        case 0b0101: // WRITE
            instMemory.write(stackRegisters.pop(), stackRegisters.pop());
            break;
        case 0b0110: // LDC
            pc.setValue(stackRegisters.pop() - 1);
            break;
        case 0b0111: // FST
            noStop = false;
            break;
        case 0b1000: // SWP
            stackRegisters.swap();
            break;
        case 0b1001: // ROR
            stackRegisters.ror();
            break;
        case 0b1010: // ROL
            stackRegisters.rol();
            break;
        case 0b1011: //CMP
            if (stackRegisters.compare())
            {
                flags.incrementBy(1);
            }
            break;
        case 0b1100: // DUP
            stackRegisters.push(stackRegisters.peek());
            break;
        case 0b1101: // JZ
            pc.incrementBy(1);
            if (flags.getValue() == 1)
            {
                flags.setValue(0);
                stackRegisters.push(instMemory.read(pc.getValue()));
                pc.setValue(stackRegisters.pop() - 1);
            }
            break;
        case 0b1110: // INC
            stackRegisters.increment(1);
            break;
        case 0b1111: // OUT
            std::cout << stackRegisters.peek();
            break;
        }
        pc.incrementBy(1);
    }
}
