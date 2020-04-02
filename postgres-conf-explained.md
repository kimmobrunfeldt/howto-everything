# Postgres performance / memory tuning for OSM

**WARNING: DO NOT copy these settings as is to your setup, it is meant for OSM data hosting.** Instead, read the comments in the conf and figure out what suits your setup.


These settings have been iterated to suit hosting a Postgres(+postgis) with OSM planet data imported. When planet data is imported to PG, Postgres data directory takes around ~500GB disk space.

The settings are tuned for:

* Fast data import stage (needs a lot of RAM).
* Performance, fsync and synchronous_commit are off because we don't need to worry about data loss.
* Write once (import), read-only when hosting.


```conf
# Intended for server with 128GB RAM
#
# Some useful links:
# https://www.postgresql.org/docs/9.6/static/runtime-config-resource.html
# https://help.openstreetmap.org/questions/24343/tuning-postgresql
# https://wiki.openstreetmap.org/wiki/PostgreSQL
# https://wiki.postgresql.org/wiki/Tuning_Your_PostgreSQL_Server
# https://wiki.openstreetmap.org/wiki/Osm2pgsql/benchmarks
# https://github.com/jfcoz/postgresqltuner


# Basic settings
data_directory = '/var/lib/postgresql/10/main'		# use data in another directory
hba_file = '/etc/postgresql/10/main/pg_hba.conf'	# host-based authentication file
ident_file = '/etc/postgresql/10/main/pg_ident.conf'	# ident configuration file
port = 5432
# If external_pid_file is not explicitly set, no extra PID file is written.
external_pid_file = '/var/run/postgresql/10-main.pid'

# "default 100 was too low for 8 rendering-procs"
# https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
max_connections = 300

# "The recommended setting is 25% of RAM with a maximum of 8GB."
# Source: https://stackoverflow.com/questions/30328861/most-impactful-postgres-settings-to-tweak-when-host-has-lots-of-free-ram
# Source: http://rhaas.blogspot.com/2012/03/tuning-sharedbuffers-and-walbuffers.html
#
# "raise shared_buffers to 30-40% of available ram"
# Source: https://help.openstreetmap.org/questions/24343/tuning-postgresql
#
#
shared_buffers = 38GB

# https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
work_mem = 1GB

# https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
maintenance_work_mem = 4GB

# "SSDs and other memory-based storage can often process many concurrent requests, so the best
#  value might be in the hundreds."
# Source: https://www.postgresql.org/docs/9.6/static/runtime-config-resource.html
#
# "effective_io_concurrency = 30 (for 4 SSDs in HW-RAID0). no performance-increase at values>30"
# Source: https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
effective_io_concurrency = 30

# We can turn these off because we don't really care if our data is lost. In emergency
# it's always possible to reinstall a new server, speed is more important than reliability
# as the master data is in the planet data sets anyways
# https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
fsync = off
synchronous_commit = off

# "While the documentation on wal_buffers suggests the default of 64 KB is sufficient so long
# as no single transaction exceeds that value, in practice write-heavy benchmarks see optimal
# performance at higher values than you might expect from that--at least 1 MB or more. With the
# only downside being increased use of shared memory, and since there's no case where more than a
# single WAL segment could need to be buffered, given modern server memory sizes the normal thing
# to do nowadays is to just set the following:
# Source: PostgreSQL 9.6 High Performance by Gregory Smith, Ibrar Ahmed:
wal_buffers = 16MB

# https://wiki.openstreetmap.org/wiki/PostgreSQL guides to use checkpoint_segments = 20, but
# Postgres 9.5 has changed the way to configure that. Nowadays you need to configure it via this
# max_wal_size option.
# https://www.postgresql.org/docs/9.6/static/release-9-5.html says that
# If you previously adjusted checkpoint_segments, the following formula will give you an
# approximately equivalent setting: max_wal_size = (3 * checkpoint_segments) * 16MB
# Thus (3 * 20) * 16MB = 960MB ~= 1GB
#
# But then I found this: https://wiki.openstreetmap.org/wiki/Osm2pgsql/benchmarks#Dell_PowerEdge_R520_2_x_Xeon_E5-2420.401.9Ghz_.2812_cores.29_128GB_RAM
# bench mark where they use "checkpoint_segments = 400".
# Sounded like a lot (just my guess) so I set it to something in between, using 100 as the old value
# we'd get (3 * 100) * 16MB = 4800MB ~= 5GB
max_wal_size = 5GB

# https://wiki.openstreetmap.org/wiki/Osm2pgsql/benchmarks#Dell_PowerEdge_R520_2_x_Xeon_E5-2420.401.9Ghz_.2812_cores.29_128GB_RAM
checkpoint_timeout=1h

# "effective_cache_size .75xRAM"
# Source: https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
effective_cache_size = 78GB

# "random_page_cost 1.1 (only for SSD users)"
# https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
random_page_cost = 1.5

# Added because the default value because:
# "Although the system will let you set random_page_cost to less than seq_page_cost,
# it is not physically sensible to do so."
#
# I think it's then better to also have this explicitly here even though it is the default
#
# Source: https://www.postgresql.org/docs/9.5/static/runtime-config-query.html
seq_page_cost = 1.0

# Helps to identify slow queries.
# "Causes the duration of each completed statement to be logged if the statement ran for at
# least the specified number of milliseconds."
log_min_duration_statement = 500

# "Auto-vacuuming means PostgreSQL starts cleaning up your data. This might slow down your data
# inserting and reading considerably. If you do not use the database for anything else, you can
# definitely turn autovacuuming off."
# Source: # https://wiki.openstreetmap.org/wiki/PostgreSQL
#
# BUT, we don't have a mechanism to set it on after the import so we'll just leave it on.
# It increases the import time but at least we're safe in that matter.
# Source: https://wiki.openstreetmap.org/wiki/User:Species/PostGIS_Tuning
autovacuum = on
