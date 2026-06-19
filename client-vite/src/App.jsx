import { useEffect, useState } from 'react'
import './App.css'
import UserProfile from './components/UserProfile'
import AddUser from './components/AddUser'
import SearchUsers from './components/SearchUsers'
import FriendsList from './components/FriendsList'
import GraphAnalytics from './components/GraphAnalytics'
import CheckConnection from './components/CheckConnection'
import CompleteNetwork from './components/CompleteNetwork'

const App = () => {
  const [activeTab, setActiveTab] = useState('dashboard')
  const [users, setUsers] = useState([])
  const [selectedUser, setSelectedUser] = useState(null)
  const [searchResults, setSearchResults] = useState([])
  const [sourceUser, setSourceUser] = useState(null)
  const [targetUser, setTargetUser] = useState(null)
  const [connectionResult, setConnectionResult] = useState("Select User 1 and User 2")
  const [maxSeparation, setMaxSeparation] = useState(0)
  const [farthestUser, setFarthestUser] = useState("---")

  const fetchUsers = async () => {
    try {
      const response = await fetch('http://localhost:3500/api/users')
      if (!response.ok) {
        throw new Error('Network response was not ok')
      }

      const result = await response.json()

      setUsers(result)
      setSelectedUser(result[0])
    } catch (error) {
      console.error('Error fetching data:', error.message)
    } 
  }
  
  useEffect(() => {
    fetchUsers()
  }, [])

  const addUser = async (newUser) => {

    try {
      const response = await fetch('http://localhost:3500/api/users', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json', 
        },
        body: JSON.stringify(newUser)
      })

      if (!response.ok) {
        throw new Error(`${response.status} ${response.statusText}`)
      }

      fetchUsers()

    } catch (error) {
        alert(`Error posting data: ${error.message}.`)
    } 
  }

  const deleteUser = async (userId) => {
    try {
        const response = await fetch(`http://localhost:3500/api/users/${userId}`, {
            method: 'DELETE', 
        })

        if (!response.ok) {
          throw new Error(`${response.status} ${response.statusText}`)
        }

        fetchUsers()

    } catch (error) {
      alert(`Error deleting data: ${error.message}`)
    }
  }

  const addFriend = async (userId, friendId) => {
    try {
      const response = await fetch('http://localhost:3500/api/connections', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json', 
        },
        body: JSON.stringify({
          userId : userId, 
          friendId: friendId
        })
      })

      if (!response.ok) {
        throw new Error(`${response.status} ${response.statusText}`)
      }

      fetchUsers()

    } catch (error) {
        alert(`Error posting data: ${error.message}.`)
    } 
  }

  const removeFriend = async (userId, friendId) => {
    try {
        const response = await fetch(`http://localhost:3500/api/connections`, {
          method: 'DELETE', 
          headers: {
          'Content-Type': 'application/json', 
          },
          body: JSON.stringify({
            userId : userId, 
            friendId: friendId
          })
        })

        if (!response.ok) {
          throw new Error(`${response.status} ${response.statusText}`)
        }

        fetchUsers()

    } catch (error) {
      alert(`Error deleting data: ${error.message}`)
    }
  }

  const handleSearch = (query) => {
    if (!query.trim()) {
      setSearchResults([])
      return
    }
    const results = users.filter(u =>
      u.name.toLowerCase().includes(query.toLowerCase()) ||
      u.hobby.toLowerCase().includes(query.toLowerCase())
    )
    setSearchResults(results)
  }

  const checkConnection = async (e) => {
    e.preventDefault();
    
    const sourceIdx = users.findIndex((user) => user._id === sourceUser)
    const sourceId = users[sourceIdx]._id
    const targetIdx = users.findIndex((user) => user._id === targetUser)
    const targetId = users[targetIdx]._id

    try {
      const response = await fetch(`http://localhost:3500/api/connections/${sourceId}/${targetId}`)
      if(!response.ok) {
        throw new Error(`${response.status} ${response.statusText}`)
      }

      const result = await response.json()
      
      if(!result.connected) setConnectionResult(result.message)

      else setConnectionResult(result.path)
      
    } catch(error) {
        console.error('Error fetching data:', error.message)
    }
  }

  return (
    <div className="app">
      <header className="app-header">
        <h1>🌐 Social Network Graph</h1>
        <p>Manage your social connections and visualize your network</p>
      </header>

      <nav className="app-nav">
        <button
          className={`nav-btn ${activeTab === 'dashboard' ? 'active' : ''}`}
          onClick={() => setActiveTab('dashboard')}
        >
          Dashboard
        </button>
        <button
          className={`nav-btn ${activeTab === 'add-user' ? 'active' : ''}`}
          onClick={() => setActiveTab('add-user')}
        >
          Add User
        </button>
        <button
          className={`nav-btn ${activeTab === 'search' ? 'active' : ''}`}
          onClick={() => setActiveTab('search')}
        >
          Search
        </button>
        <button
          className={`nav-btn ${activeTab === 'network' ? 'active' : ''}`}
          onClick={() => setActiveTab('network')}
        >
          Complete Network
        </button>
        <button
          className={`nav-btn ${activeTab === 'analytics' ? 'active' : ''}`}
          onClick={() => setActiveTab('analytics')}
        >
          Analytics
        </button>
        <button
          className={`nav-btn ${activeTab === 'check-connection' ? 'active' : ''}`}
          onClick={() => setActiveTab('check-connection')}
        >
          Check Connection
        </button>
      </nav>

      <main className="app-main">
        {activeTab === 'dashboard' && (
          <div className="dashboard-container">
            { selectedUser && <UserProfile
              user={selectedUser}
              allUsers={users}
              onUserSelect={setSelectedUser}
              onAddFriend={addFriend}
              onRemoveFriend={removeFriend}
              onDeleteUser={deleteUser} 
            /> }
            { selectedUser && <FriendsList
              user={selectedUser}
              users={users}
              maxSeparation={maxSeparation} 
              setMaxSeparation={setMaxSeparation} 
              farthestUser={farthestUser} 
              setFarthestUser={setFarthestUser} 
              onUserSelect={setSelectedUser}
            /> }
          </div>
        )}

        {activeTab === 'add-user' && (
          <AddUser onAddUser={addUser} />
        )}

        {activeTab === 'search' && (
          <SearchUsers
            onSearch={handleSearch}
            results={searchResults}
            onUserSelect={setSelectedUser}
            onSelectTab={() => setActiveTab('dashboard')}
          />
        )}

        {activeTab === 'network' && (
          <CompleteNetwork user={selectedUser} />
        )}

        {activeTab === 'analytics' && (
          <GraphAnalytics users={users} />
        )}

        {activeTab === 'check-connection' && (
          <CheckConnection 
            sourceUser={sourceUser} 
            setSourceUser={setSourceUser} 
            users={users} 
            targetUser={targetUser} 
            setTargetUser={setTargetUser} 
            checkConnection={checkConnection}
            connectionResult={connectionResult}
          />
        )}
      </main>

      <footer className="app-footer">
        <p>Total Users: {users.length} | Total Connections: {users.reduce((sum, u) => sum + u.friends.length, 0) / 2}</p>
      </footer>
    </div>
  )
}

export default App
