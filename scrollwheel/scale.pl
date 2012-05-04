#!/usr/bin/perl -W

sub usage();

if(scalar(@ARGV) < 4) {
	usage();
}

my $outfile = $ARGV[1];
my $size = $ARGV[3]; # in 'mm' or in 'in'
my $factor;
my $layer = $ARGV[2];

open(FILE, '<', $ARGV[0]) || die("cannot open: $!");
my @infiledata = <FILE>;
close(FILE);

if($size =~ m/^\d{1,3}\.\d{1,3}in$/) {
	$size =~ s/in//;
	$factor = 1/1.18*$size;
} elsif($size =~ m/^\d{1,3}\.\d{1,3}mm$/) {
	$size =~ s/mm//;
	$factor = 1/1.18/25.4*$size;
} else {
	usage();
}

open(FILE,'>',$outfile) || die("cannot open: $!");
foreach $line (@infiledata) {
	my $tmp = $line;
	if($tmp =~ m/^(.*Dl)\ (.*)\ (.*)$/) {
		print FILE $1," ",int($2*$factor)," ",int($3*$factor),"\n";
	} elsif($tmp =~ m/^T(\d{1})\ 0\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)(.*)$/) {
		print FILE "T$1 0 ",int($2*$factor)," ",int($3*$factor)," ",int($4*$factor)," ",int($5*$factor)," ",int($6*$factor),$7,"\n";
	} elsif($tmp =~ m/^(.*DP)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)\ (-{0,1}\d*)(.*)$/) {
		print FILE "$1 $2 $3 $4 $5 $6 $7 $layer\n";
	}
	else {
		print FILE $line;
	}
}

close(FILE);

sub usage() {
	print "\nusage: scale.pl <infile.emp> <outfile.emp> <layer number> <size: e.g. 5.00mm or 0.25in>\n\n";
	print "KiCad layers:\n";
	print "-------------\n\n";
	print "Top copper:        15\n";
	print "Top silkscreen:    21\n\n";
	print "To move the logo to bottom copper/silkscreen\n";
	print "move the curser over it and press F for flip layer\n\n";
	exit;
}

