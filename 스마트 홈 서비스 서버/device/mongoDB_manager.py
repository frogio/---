import pymongo

class MongoDBManager:                                                   # Singleton 클래스
    __instance = None                                                   # 언더바 _를 두개 붙히면 private
    client = pymongo.MongoClient(host="localhost", port = 27017)        # 서버 접속정보
    database = None                                                     # DB 이름, 콜렉션 이름 

    def __new__(cls, *args, **kwargs):
        if (cls.__instance is None):
            cls.__instance = object.__new__(cls, *args, **kwargs)

        return cls.__instance
        
    def printDBLog(cls, msg):
        print("Database> " + msg)


    def get_data(cls, query, collection_name):                  # 데이터를 mongoDB의 collection으로부터 가져옴
        cls.database = cls.client["SmartHomeService"][collection_name] 
        
        if(cls.database is not None):                           # 데이터베이스가 None이 아닐 때 query로 데이터를 가져옴
            cls.printDBLog("retrieve measured sensor value from DB")
            cls.printDBLog("query info : " + str(query))
            return cls.database.find(query)

            
    def add_data(cls, data, collection_name):                   # 데이터를 mongoDB의 collection에 추가함

        cls.database = cls.client["SmartHomeService"][collection_name]

        if(type(data) is list):
            cls.printDBLog("insert measured sensor value list into DB")
            # cls.printDBLog("data info : " + str(data))
            return cls.database.insert_many(data)

        else:
            cls.printDBLog("append measured sensor value to DB")
            # cls.printDBLog("data info : " + str(data))
            return cls.database.insert_one(data)