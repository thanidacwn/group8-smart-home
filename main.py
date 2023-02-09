from fastapi import FastAPI
from routers import rooms
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()
app.include_router(rooms.router)

origins = ["*"]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def root():
    """ Root page of the API """
    return {"msg": "welcome to group8 smart home root page สุดเท่"}
