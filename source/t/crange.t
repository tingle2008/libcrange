#!/usr/bin/perl -w

use warnings;
use strict;

use Test::More;
use FindBin;
use File::Temp;

my $config_base = "$FindBin::Bin/test_configs/crange1";
my $build_root = $ENV{DESTDIR};
my ($range_conf_fh, $range_conf) = File::Temp::tempfile();
warn "tmpfile: $range_conf";

#FIXME allow setting of lr->funcdir in range.conf
# to let me funcdir=$build_root/usr/lib/libcrange
# and remove other refs to $build_root
my $range_conf_data = qq{
dns_data_file=$config_base/etc/dns_data.tinydns
yst_ip_list=$config_base/etc/yst-ip-list
yaml_path=$config_base/rangedata
loadmodule $build_root/usr/lib/libcrange/yamlfile
loadmodule $build_root/usr/lib/libcrange/ip
loadmodule $build_root/usr/lib/libcrange/yst-ip-list
};

print $range_conf_fh $range_conf_data;


# just shasum outputs for now to make sure we're returning consistent data
is( `crange -e foo100..10|shasum`,
    "74ac488e1d74ffffbf57f161d5f336069b96f410  -\n",
    "foo100..10 # noconfig");

if ($^O ne "darwin") {
# OSX getopt doesn't allow this ordering of args
    is(
        `crange  -c $range_conf foo100..1 -e 2>&1`,
        qq{foo100\nfoo101\n},
        "foo100..1 # using range.conf",
        );
}

is(
    `crange -e -c $range_conf foo100..1 2>&1`,
    qq{foo100\nfoo101\n},
    "foo100..1 # using range.conf",
    );


is(
  `crange  -c $range_conf -e  'vlan(foo1.example.com)'`,
  qq{"1.2.3.0/24"\n},
  "vlan(foo1.example.com)",
  );


is(
  `crange  -c $range_conf -e  '\@bar'`,
  qq{foo1.example.com\nfoo2.example.com\n},
  '@bar',
  );

is(
  `crange  -c $range_conf -e  'has(bar;foo1.example.com)'`,
  qq{GROUPS\n},
  'has(bar;foo1.example.com) # should work',
  );

my @arg_needing_funcs = qw(
  mem cluster clusters group get_cluster get_groups has 
  vlan dc hosts_v hosts_dc vlans_dc ip group
);

my @no_arg_funcs = qw(
  allclusters 
);

for my $func (@arg_needing_funcs) {
  is(
    `crange  -c $range_conf -e '$func()'`,
    qq{},
    "$func() # should return empty",
    );
}

done_testing();
