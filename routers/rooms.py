from fastapi import APIRouter, Body, HTTPException
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel
from database.mongo_connection import *

router = APIRouter(
    prefix="",
    tags=["rooms"],
    responses={404: {"description": "Not found"}},
)

class Room(BaseModel):
    """ Room model """
    state: int
    brightness: int
    is_auto: int

class Data(BaseModel):
    """ Data model """
    room1: Room
    room2: Room
    room3: Room


def binary_to_percentage(binary):
    """ Convert binary to percentage """
    return round(binary / 2.55)

def percentage_to_binary(percentage):
    """ Convert percentage to binary """
    return round(percentage * 2.55)

def check_exception(data: Room):
    if data.state not in [0, 1]:
        raise HTTPException(status_code=400, detail="Incorrect state.")
    if data.brightness not in range(0,101):
        raise HTTPException(status_code=400, detail="Incorrect brightness.")
    if data.is_auto not in [0, 1]:
        raise HTTPException(status_code=400, detail="Incorrect status for light control")

@router.get("/rooms")
def get_all_rooms_from_hardware():
    """ Get all rooms """
    room = {}
    list_rooms = ["room1", "room2", "room3"]
    for i in range(1, 4):
        data = collection.find_one({"room_num": i}, {"_id": False})
        new_data = {
            "state": data["state"],
            "brightness": data["brightness"],
            "is_auto": data["is_auto"]
        }
        room[list_rooms[i - 1]] = new_data
    return room

@router.post("/update_rooms")
def update_all_rooms_from_hardware(data: Data):
    """ Update all rooms """
    result = {}
    lst_data = [data.room1, data.room2, data.room3]
    for i in range(1,4):
        room = lst_data[i-1]
        check_exception(room)
        collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        old = collection.find_one({"room_num": i}, {"_id": False})
        if room.is_auto == 1 and (old["state"] != room.state):
            raise HTTPException(status_code=400, detail="Light is in auto mode. Can not manually switched the light.")
        else:
            collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        collection.update_one({"room_num": i}, {"$set": {"brightness": room.brightness}})
        result[f"room{i}"] = collection.find_one({"room_num": i}, {"_id": False})
    return {"results": result}

@router.post("/web_data")
def update_data_from_front(data: Data):
    result = {}
    lst_data = [data.room1, data.room2, data.room3]
    for i in range(1,4):
        room = lst_data[i-1]
        check_exception(room)
        collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        old = collection.find_one({"room_num": i}, {"_id": False})
        if room.is_auto == 1 and (old["state"] != room.state):
            raise HTTPException(status_code=400, detail="Light is in auto mode. Can not manually switched the light.")
        else:
            collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        collection.update_one({"room_num": i}, {"$set": {"brightness": percentage_to_binary(room.brightness)}})
        result[f"room{i}"] = collection.find_one({"room_num": i}, {"_id": False})
    return {"results": result}
 
@router.get("/update_web_data")
def update_web_data():
    room = {}
    list_rooms = ["room1", "room2", "room3"]
    for i in range(1, 4):
        data = collection.find_one({"room_num": i}, {"_id": False})
        new_data = {
            "state": data["state"],
            "brightness": binary_to_percentage(data["brightness"]),
            "is_auto": data["is_auto"]
        }
        room[list_rooms[i - 1]] = new_data
    return room