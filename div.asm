IMV #0
TXA R1

IMV #51
TXA R2

IMV #65535
TXA R3

.divloop
SUB R2
IAT .divloop2
BRC
ADD R2
IAT .end
JMP
.divloop2
TXA R3
RXA R1
INC
TXA R1
RXA R3
IAT .divloop
JMP

.end
RXA R1
IAH #128
IAL #0
STR
HLT