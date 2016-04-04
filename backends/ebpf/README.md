This is a back-end which generates C code that can be afterwards
compiled into eBPF (extended Berkeley Packet Filters
https://en.wikipedia.org/wiki/Berkeley_Packet_Filter) using clang/llvm
or bcc (https://github.com/iovisor/bcc.git).

The back-end accepts code written for the ebpf_model.p4 filter model.
