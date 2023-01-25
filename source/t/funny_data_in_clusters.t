#!/usr/bin/perl -w

use warnings;
use strict;

use Test::More qw(no_plan);
use FindBin;
use File::Temp;

my $config_base = "$FindBin::Bin/test_configs/funny_data_in_clusters";
my $build_root = $ENV{DESTDIR};
my ($range_conf_fh, $range_conf) = File::Temp::tempfile();

#FIXME should probably sort outputs for stable tests

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

# just md5sum outputs for now to make sure we're returning consistent data
like( `crange -c $range_conf -e '\%urls:bare'`,
    qr/syntax error, unexpected tREGEX/,
    "\%urls:bare # funky entries",
    );

is( `crange -c $range_conf -e '\%urls:dquote'`,
    qq{"http://doublequotes.example.com/"\n},
    "\%urls:dquote # double quoted url",
    );

is( `crange -c $range_conf -e '\%urls:qfunc'`,
    qq{"http://qfunc.example.com/"\n},
    "\%urls:qfunc # q() quoted url",
    );

like( `crange -c $range_conf %emptyclusterfile`,
      qr/emptyclusterfile: malformatted cluster definition/,
      "%emptyclusterfile, a 0-byte cluster yamlfile",
      );
