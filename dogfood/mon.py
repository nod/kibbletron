import time


import redis

r = redis.Redis(host=REDIS_HOST, db=0)

while True:
    print("kibbletron:", r.get("kibbletron"))
    print("dogfood:", r.get("dogfood"))
    time.sleep(2.0)


