from fastapi import FastAPI
from routers import rooms

app = FastAPI()
app.include_router(rooms.router)


@app.get("/")
def root():
    """ Root page of the API """
    return {"msg": "welcome to group8 smart home root page สุดเท่"}
