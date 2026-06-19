import { useEffect } from 'react'
import './FriendsList.css'

const calculateNetworkReach = (user, users) => {
  const visited = new Set([user._id])
  const queue = [user._id]

  while (queue.length > 0) {
    const userId = queue.shift()
    const currentUser = users.find(u => u._id === userId)

    if (currentUser) {
      for (const friendId of currentUser.friends) {
        if (!visited.has(friendId)) {
          visited.add(friendId)
          queue.push(friendId)
        }
      }
    }
  }

  return visited.size
}


const FriendsList = ({ user, users, maxSeparation, setMaxSeparation, farthestUser, setFarthestUser, onUserSelect }) => {

  const friends = users.filter(u => user.friends.includes(u._id))

  const getFarthestFriend = async (userId) => {
    try {
      const response = await fetch(`http://localhost:3500/api/users/${userId}/max-separation`)
      if(!response.ok) {
        throw new Error('Network response was not ok')
      }
  
      const result = await response.json()
      setFarthestUser(result.farthestUser)
      setMaxSeparation(result.degree)
  
    } catch(error) {
      console.error('Error fetching data:', error.message)
    }
  }

  useEffect(() => {
    getFarthestFriend(user._id)
  }, [])

  const handleSelectFriend = (friendId) => {
    getFarthestFriend(friendId)
    const friend = users.find(u => u._id === friendId)
    onUserSelect(friend)
  }

  return (
    <div className="friends-list">
      <div className="friends-header">
        <h3>Friends ({friends.length})</h3>
      </div>

      {friends.length === 0 ? (
        <div className="no-friends">
          <p>No friends yet. Add some friends to get started!</p>
        </div>
      ) : (
        <div className="friends-container">
          {friends.map(friend => (
            <div
              key={friend._id}
              className="friend-card"
              onClick={() => handleSelectFriend(friend._id)}
            >
              <div className="friend-avatar">{friend.name.charAt(0)}</div>
              <div className="friend-details">
                <h4>{friend.name}</h4>
                <p>{friend.hobby}</p>
                <small>{friend.friends.length} mutual friends</small>
              </div>
              <div className="friend-arrow">→</div>
            </div>
          ))}
        </div>
      )}

      <div className="friends-stats">
        <div className="stat-item">
          <span className="stat-icon">👥</span>
          <div>
            <p className="stat-label">Network Reach</p>
            <p className="stat-value">{calculateNetworkReach(user, users)} users</p>
          </div>
        </div>
        <div className="stat-item">
          <span className="stat-icon">🔗</span>
          <div>
            <p className="stat-label">Direct Connections</p>
            <p className="stat-value">{user.friends.length}</p>
          </div>
        </div>
        <div className="stat-item">
          <span className="stat-icon">🌍</span>
          <div>
            <p className="stat-label">Farthest Connection</p>
            <p className="stat-value">{farthestUser}</p>
            <small>{maxSeparation} steps away</small>
          </div>
        </div>
      </div>
    </div>
  )
}

export default FriendsList
