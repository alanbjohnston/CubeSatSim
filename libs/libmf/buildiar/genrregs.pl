#!/usr/bin/perl

my %regs;

while (<>) {
    next unless /^\s*SFRX\(\s*([A-Za-z0-9_]+)\s*,\s*(0x(?:[0-9A-Fa-f])+)\s*\)/;
    my $name = $1;
    my $addr = hex($2);
    next if $addr < 0x4000 || $addr > 0x5fff;
    #printf "%s 0x%x\n", $name, $addr;
    ${$regs{$addr}}{$name} = 1;
}

# foreach $addr (sort keys(%regs)) {
#     printf "0x%x:\n", $addr;
#     foreach $name (sort keys(%{$regs{$addr}})) {
# 	printf "%s 0x%x\n", $name, $addr;
#     }
# }

printf "\tMODULE radioregs\n\n", $addr;
foreach $addr (sort keys(%regs)) {
    foreach $name (sort keys(%{$regs{$addr}})) {
	printf "\tPUBWEAK %s\n", $name;
    }
    printf "\n\tASEGN XDATA_AN:XDATA:ROOT,%05XH\n", $addr;
    foreach $name (sort keys(%{$regs{$addr}})) {
	printf "%s:\n", $name;
    }
    printf "\tDATA8\n\tDS 1\n\n";
}
print "\tEND\n\n";
