import time


import redis

r = redis.Redis(host=REDIS_HOST, db=0)

r.set("dogfood", "eat")


