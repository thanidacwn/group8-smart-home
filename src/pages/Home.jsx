import React, { useEffect, useState } from 'react'
import { getRoom } from '../services/rooms'
import { postRoom } from '../services/rooms'
import Card from '../components/Card'

const Home=()=> {
    const [roomInfo, setRoomInfo] = useState({
        room1: {
          state: 0,
          brightness: 0,
          is_auto: 0,
        },
        room2: {
          state: 0,
          brightness: 0,
          is_auto: 0,
        },
        room3: {
          state: 0,
          brightness: 0,
          is_auto: 0,
        },
    })
    
    useEffect(() => {
        setInterval(() => {
            getRoom().then(data => setRoomInfo(data))
        },1000)
    }, [])

    const handleChangeInfo = (room, state, brightness, is_auto) => {
        setRoomInfo({...roomInfo, })
    }
    
    return (
        <div>
            <h1>Home</h1>
            <Card brightness={0} is_auto={true} state={false}/>
            <Card brightness={100} is_auto={false} state={true}/>
            <Card brightness={0} is_auto={true} state={false}/>
        </div>
    )
}
export default Home