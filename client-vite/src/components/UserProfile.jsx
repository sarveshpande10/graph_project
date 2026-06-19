import { useState } from 'react'
import './UserProfile.css'

const UserProfile = ({ 
  user, 
  allUsers, 
  onUserSelect, 
  onAddFriend, 
  onRemoveFriend, 
  onDeleteUser, 
 }) => {

  const isFriend = (userId) => user.friends.includes(userId)

  const handleAddFriend = (friendId) => {
    onAddFriend(user._id, friendId)
  }

  const handleRemoveFriend = (friendId) => {
    onRemoveFriend(user._id, friendId)
  }

  const suggestedFriends = allUsers.filter(
    u => u._id !== user._id && !user.friends.includes(u._id)
  )

  const currentMates = allUsers.filter(other => {
  if (other._id === user._id) return false;

  return user.educationalInstitutes?.some(myInst =>
    myInst.currentlyStudying &&
    other.educationalInstitutes?.some(
      inst =>
        inst.currentlyStudying &&
        inst.instituteName === myInst.instituteName
    )
  )
})

const pastMates = allUsers.filter(other => {
  if (other._id === user._id) return false;  

  if (user.educationalInstitutes[0]?.instituteName === "" || other.educationalInstitutes[0]?.instituteName === "") {
    return false;
  }

  return user.educationalInstitutes.some(myInst =>
    myInst.currentlyStudying === false &&
    other.educationalInstitutes.some(inst =>
      inst.currentlyStudying === false &&
      inst.instituteName === myInst.instituteName
    )
  )
})



  return (
    <div className="user-profile">
      <div className="profile-header">
        <div className="profile-avatar">{user.name.charAt(0)}</div>
        <div className="profile-info">
          <h2>{user.name}</h2>
          <p className="hobby">{user.hobby}</p>
        </div>
        <button className="delete-btn" onClick={() => onDeleteUser(user._id)}>
          Delete User
        </button>
      </div>

      <div className="profile-stats">
        <div className="stat">
          <span className="stat-value">{user.friends.length}</span>
          <span className="stat-label">Friends</span>
        </div>
      </div>

      <div className="suggested-friends">
        <h3>Suggested Friends</h3>
        {suggestedFriends.length > 0 ? (
          <div className="suggestions-list">
            {suggestedFriends.map(friend => (
              <div key={friend._id} className="suggestion-item">
                <div className="suggestion-info">
                  <p>{friend.name}</p>
                  <small>{friend.email}</small>
                </div>
                <button
                  className="add-btn"
                  onClick={() => handleAddFriend(friend._id)}
                >
                  Add
                </button>
              </div>
            ))}
          </div>
        ) : (
          <p className="no-suggestions">No more users to add</p>
        )}
      </div>

      <div className="mates-section">
        <h3>Current Mates</h3>

        {currentMates.length > 0 ? (
          <div className="users-list">
            {currentMates.map(mate => (
              <div key={mate._id} className="user-item mate-item">
                <div className="user-details">
                  <p className="user-name">{mate.name}</p>
                  <small>
                    Currently at: {
                      mate.educationalInstitutes.find(
                        i => i.currentlyStudying
                      )?.instituteName
                    }
                  </small>
                </div>

                {isFriend(mate._id) ? (
                  <button
                    className="remove-btn"
                    onClick={() => handleRemoveFriend(mate._id)}
                  >
                    Remove
                  </button>
                ) : (
                  <button
                    className="add-btn"
                    onClick={() => handleAddFriend(mate._id)}
                  >
                    Add Friend
                  </button>
                )}
              </div>
            ))}
          </div>
        ) : (
          <p className="no-suggestions">
            No current mates found
          </p>
        )}
      </div>

      <div className="mates-section">
        <h3>Past Mates</h3>

        {pastMates.length > 0 ? (
          <div className="users-list">
            {pastMates.map(mate => (
              <div key={mate._id} className="user-item mate-item">
                <div className="user-details">
                  <p className="user-name">{mate.name}</p>
                  <small>
                    Studied together at: {
                      mate.educationalInstitutes.find(
                        i => !i.currentlyStudying
                      )?.instituteName
                    }
                  </small>
                </div>

                {isFriend(mate._id) ? (
                  <button
                    className="remove-btn"
                    onClick={() => handleRemoveFriend(mate._id)}
                  >
                    Remove
                  </button>
                ) : (
                  <button
                    className="add-btn"
                    onClick={() => handleAddFriend(mate._id)}
                  >
                    Add Friend
                  </button>
                )}
              </div>
            ))}
          </div>
        ) : (
          <p className="no-suggestions">
            No past mates found
          </p>
        )}
      </div>

      <div className="profile-actions">
        <h3>Quick Actions</h3>
        <div className="users-list">
          {allUsers.map(u => (
            u._id !== user._id && (
              <div key={u._id} className="user-item">
                <div className="user-details">
                  <p className="user-name">{u.name}</p>
                  <small>{u.email}</small>
                </div>
                {isFriend(u._id) ? (
                  <button
                    className="remove-btn"
                    onClick={() => handleRemoveFriend(u._id)}
                  >
                    Remove
                  </button>
                ) : (
                  <button
                    className="add-btn"
                    onClick={() => handleAddFriend(u._id)}
                  >
                    Add Friend
                  </button>
                )}
              </div>
            )
          ))}
        </div>
      </div>
    </div>
  )
}

export default UserProfile
