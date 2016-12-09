PGPASSWORD="123" pg_dump -h 192.168.100.132 -d postgres -p 5432 -U postgres -t t_trans_conf > ./test.sql
