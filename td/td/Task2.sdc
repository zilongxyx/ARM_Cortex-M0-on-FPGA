create_clock -name System_clk -period 20 -waveform {0 10} [get_ports { System_clk }]
create_clock -name clk -period 40 -waveform {0 20} [get_nets { clk }]