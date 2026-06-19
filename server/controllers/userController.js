const User = require('../model/User') 
const { getShortestPathTree } = require('./connectionController')

const getUserMap = async () => {
    const users = await User.find().lean();

    const userMap = new Map();

    users.forEach(user => {
        userMap.set(user._id.toString(), user);
    });

    return userMap;
};

const getAllUsers = async (req, res) => {
    try {
        const allUsers = await User.find().exec()
        res.json(allUsers)
    } catch(err) {
        res.status(500).json({ message: err.message })
    }
}

const postUser = async (req, res) => {
    if(!req.body?.name) return res.status(400).json({ message: 'Username is required.' })

    try {
        const newUser = await User.create(req.body)
        res.status(201).json(newUser)
    } catch(err) {
        res.status(400).json({ message: err.message })
    }
}

const deleteUser = async (req, res) => {
    if(!req?.params.id) return res.status(400).json({ message: 'User ID is required.' })

    try {
        const user = await User.findById(req.params.id)
        if(!user) {
                return res.status(404).json({ message: 'User not found.' })
        }
        for (const friend of user.friends) {
            await User.updateOne(
                { _id: friend },
                {
                    $pull: {
                        friends: req.params.id
                    }
                }
            );
        }

        await user.deleteOne()
        res.sendStatus(204)
    } catch(err) {
        res.status(400).json({ message: err.message })
    }
}

const getMaximumPathOfSeparation = async (req, res) => {
    if(!req?.params.id) {
        return res.status(400).json({
            message: "User ID is required"
        })
    }
    try {
        const user = await User.findById(req.params.id)
        if(!user) {
            return res.status(404).json({ message: 'User not found.' })
        }
        const userMap = await getUserMap()
        const { distance, parent } = await getShortestPathTree(req.params.id, -1, userMap)
        let maxi = 0, maxId = "";
    
        for (const id in distance) {
            if (distance[id] > maxi) {
                maxi = distance[id];
                maxId = id;
          }
        }
    
        if (maxId === "") {
            return res.json({
                farthestUser: "---", 
                degree: 0, 
                path: []
            })
        }
        const farthestFriend = userMap.get(maxId.toString())
        const path = [];
        path.push(farthestFriend.name);
        let i = maxId;
    
        while (i !== req.params.id) {
            const friend = userMap.get(parent[i].toString())
            path.push(friend.name);
            i = parent[i];
        }
        
        res.json({
            farthestUser: farthestFriend.name, 
            degree: maxi, 
            path: path.reverse() 
        })
    
    } catch(err) {
        res.status(400).json({ message: err.message })
    }
}

const getCompleteNetwork = async (req, res) => {
    if(!req?.params.id) {
        return res.status(400).json({
            message: "User ID is required"
        })
    }
    try {
        const user = await User.findById(req.params.id)
        if(!user) {
            return res.status(404).json({ message: 'User not found.' })
        }
        const userMap = await getUserMap()
        const { parent } = await getShortestPathTree(req.params.id, -1, userMap);
        const connections = [];

        for (const friendId in parent) {
            if (friendId !== req.params.id) {
                const path = [];
                const friend = userMap.get(friendId.toString())
                path.push(friend.name);
                let i = friendId;

                while (i !== req.params.id) {
                    const fartherFriend = userMap.get(parent[i].toString())
                    path.push(fartherFriend.name);
                    i = parent[i];
                }

                connections.push(path.reverse());
            }
        }

        res.json({ 
            totalConnections: connections.length,
            connections: connections 
        })
    } catch(err) {
        res.status(400).json({ message: err.message })
    }
    
  }

module.exports = {
    getAllUsers, 
    postUser, 
    deleteUser, 
    getMaximumPathOfSeparation, 
    getCompleteNetwork
}