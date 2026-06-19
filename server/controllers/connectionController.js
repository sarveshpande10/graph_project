const User = require('../model/User') 

const getUserMap = async () => {
    const users = await User.find().lean();

    const userMap = new Map();

    users.forEach(user => {
        userMap.set(user._id.toString(), user);
    });

    return userMap;
};

const addConnection = async (req, res) => {
    const {userId, friendId} = req.body
    if(!userId || !friendId) {
        return res.status(400).json({
            message: 'User ID and Friend ID are required'
        })
    }

    try {
        const user = await User.findById(userId)
        const friend = await User.findById(friendId)

        if(!user || !friend) {
            return res.status(404).json({
                message: "User not found"
            });
        }

        if (userId === friendId) {
            return res.status(400).json({
                message: "A user cannot be friends with themselves"
            });
        }

        const updateResult = await User.updateOne(
            {_id: userId},
            {
                $addToSet: {
                    friends: friendId
                }
            }
        )

        if(updateResult.modifiedCount === 0) {
            return res.status(400).json({
                message: "Users are already connected"
            });
        }

        await User.updateOne(
            {_id: friendId},
            {
                $addToSet: {
                    friends: userId
                }
            }
        )
        
        res.status(201).json({
            message: "Users connected successfully!"
        })
    } catch(err) {
        res.status(400).json({ 'message': err.message })
    }
}

const removeConnection = async (req, res) => {
    const { userId, friendId } = req.body
    if(!userId || !friendId) {
        return res.status(400).json({
            message : "User ID and Friend ID are required"
        })
    }
    try {
        const user = await User.findById(userId)
        const friend = await User.findById(friendId)
        if(!user || !friend) {
            return res.status(404).json({
                message: "User not found"
            })
        }
    
        const updateResult = await user.updateOne({
            $pull: {
                friends: friendId
            }
        })
    
        if(updateResult.modifiedCount === 0) {
            return res.status(400).json({
                message: "Users are not connected"
            })
        }

        await friend.updateOne({
            $pull: {
                friends: userId
            }
        })

        res.sendStatus(204)
    } catch(err) {
        res.status(400).json({ 'message': err.message })
    }

}

const getShortestPathTree = async (sourceId, destinationId = -1, userMap = {}) => {
    
    const distance = {};
    
    distance[sourceId] = 0;

    const queue = [sourceId];
    const parent = {};

    parent[sourceId] = sourceId;

    while (queue.length > 0) {
        const userId = queue.shift();
        if (destinationId !== -1 && userId === destinationId) break;
        
        const user = userMap.get(userId.toString())
                
        for (const friend of user.friends) {
        if (distance[friend] === undefined) {
            distance[friend] = distance[userId] + 1;
            parent[friend] = userId;
            queue.push(friend);
        }
        }
    }
    
    return { distance, parent };
}

const getConnectionPath = async (source, destination) => {
    const userMap = await getUserMap()
    const { parent } = await getShortestPathTree(source, destination, userMap);
    
    if (parent[destination] === undefined) return [];

    const path = [];
    const friend = userMap.get(destination.toString())
    path.push(friend.name);
    let i = destination;

    while (i !== source) {
        const user = userMap.get(parent[i].toString())
        path.push(user.name);
        i = parent[i];
    }

    return path;
}

const checkConnection = async (req, res) => {
    if(!req?.params.userId || !req?.params.friendId) {
        return res.status(400).json({
            message: "User ID and Friend ID are required"
        })
    }
    const { userId, friendId } = req.params

    try {
        const user = await User.findById(userId)
        const friend = await User.findById(friendId)
        if(!user || !friend) {
            return res.status(404).json({
                message: "User not found"
            })
        }

        const path = await getConnectionPath(userId, friendId)
        
        if(path.length === 0) {
            return res.json({
                connected: false,
                message: "Users are not connected"
            })
        }

        res.json({
            connected: true,
            path: path.reverse()
        })
    } catch(err) {
        res.status(400).json({ 'message': err.message })
    }
}

module.exports = {
    addConnection, 
    removeConnection, 
    checkConnection, 
    getShortestPathTree
}