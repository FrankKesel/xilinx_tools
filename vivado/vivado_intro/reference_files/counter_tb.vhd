library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity counter_tb is
end counter_tb;

architecture Behavioral of counter_tb is

component counter is
    Port ( clk : in STD_LOGIC;
           reset : in STD_LOGIC;
           count : in STD_LOGIC;
           ctr_out : out STD_LOGIC_VECTOR (3 downto 0));
end component;

signal clk, reset, count : std_logic := '0';
signal ctr_out : std_logic_vector(3 downto 0);
constant clk_period: time := 10 ns; -- 100 MHz

begin
dut: counter port map (
    clk => clk,
    reset => reset,
    count => count,
    ctr_out => ctr_out 
);

clk_proc: process
begin
    clk <= '0';
    wait for clk_period/2;
    clk <= '1';
    wait for clk_period/2;
end process;

stim_proc: process
begin
   reset <= '1';
   wait for 20 ns;
   reset <= '0';
   wait for 40 ns; 
   count <= '1';
   wait for 10 ns; 
   count <= '0';
   wait for 60 ns;
   count <= '1';
   wait;
end process;

end Behavioral;
