import { useEffect, useState } from 'react'
import './CompleteNetwork.css'

const CompleteNetwork = ({ user }) => {
  const [connections, setConnections] = useState([])
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    const fetchNetwork = async () => {
      if (!user) return

      try {
        setLoading(true)

        const response = await fetch(
          `http://localhost:3500/api/users/${user._id}/network`
        )

        if (!response.ok) {
          throw new Error('Failed to fetch network')
        }

        const result = await response.json()
        setConnections(result.connections)
      } catch (err) {
        console.error(err)
      } finally {
        setLoading(false)
      }
    }

    fetchNetwork()
  }, [user])

  return (
    <div className="network-container">
      <div className="network-header">
        <h2>🌐 Complete Network</h2>
        <p>
          Reachable users from <strong>{user?.name}</strong>
        </p>
      </div>

      {loading ? (
        <div className="network-message">
          Loading network...
        </div>
      ) : connections.length === 0 ? (
        <div className="network-message">
          No network connections found.
        </div>
      ) : (
        <div className="network-list">
          {connections.map((path, index) => (
            <div key={index} className="network-card">
              <div className="network-degree">
                Degree: {path.length - 1}
              </div>

              <div className="network-path">
                {path.map((person, idx) => (
                  <span key={idx}>
                    {person}
                    {idx !== path.length - 1 && (
                      <span className="arrow"> → </span>
                    )}
                  </span>
                ))}
              </div>
            </div>
          ))}
        </div>
      )}
    </div>
  )
}

export default CompleteNetwork