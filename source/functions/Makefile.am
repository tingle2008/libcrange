AM_CFLAGS = -g -pg -Wall -DLIBCRANGE_FUNCDIR=\"$(pkglibdir)\" \
		   			-DLIBCRANGE_CONF=\"/etc/libcrange.conf\" \
				    -DDEFAULT_SQLITE_DB=\"/var/range.sqlite\" \
					  -DLIBCRANGE_YAML_DIR=\"/var/range/\" \
						-I/usr/pgsql-11/include \
						-I../src \
						@PCRE_CFLAGS@ @APR_CFLAGS@
AM_LDFLAGS = -module -L../src -L/tmp/install/lib64 -lcrange -lyaml -lsqlite3 \
						 -L/usr/lib64/mysql -lmysqlclient \
						 -L/usr/pgsql-11/lib -lpq \
							@PCRE_LIBS@ @APR_LIBS@

pkglib_LTLIBRARIES = yst-ip-list.la ip.la nodescf.la yamlfile.la sqlite.la group-mysql.la mt5-mysql.la mt5-pgsql.la

sqlite_la_SOURCES = sqlite.c
nodescf_la_SOURCES = nodescf.c
group_mysql_la_SOURCES = group-mysql.c
mt5_pgsql_la_SOURCES = mt5-pgsql.c
mt5_mysql_la_SOURCES = mt5-mysql.c
yamlfile_la_SOURCES = yamlfile.c
yst_ip_list_la_SOURCES = yst-ip-list.c netblock.c tinydns_ip.c \
                         hosts-netblocks.c
ip_la_SOURCES = ip.c tinydns_ip.c



