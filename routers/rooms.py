from fastapi import APIRouter, Body, HTTPException
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel
from pymongo import MongoClient
from dotenv import load_dotenv
import os

load_dotenv(".env")
username = 'exceed08'
password = '427YQucG'

client = MongoClient(
    f"mongodb://{username}:{password}@mongo.exceed19.online:8443/?authMechanism=DEFAULT"
)

db = client["exceed08"]
collection = db["smart_home"]

router = APIRouter(
    prefix="",
    tags=["rooms"],
    responses={404: {"description": "Not found"}},
)

class Room(BaseModel):
    room_num: int
    state: int
    brightness: int
    is_auto: int

class Data(BaseModel):
    room1: Room
    room2: Room
    room3: Room

def binary_to_percentage(binary):
    """ Convert binary to percentage """
    return round(binary / 2.55)


@router.get("/rooms")
def get_all_rooms():
    """ Get all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms

@router.post("/update_rooms")
def update_all_rooms():
    """ Update all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms


@router.post("/web_data")
def get_web_data(data:Data):
    pass

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


