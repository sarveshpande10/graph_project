import { useState } from 'react'
import './GraphAnalytics.css'

const GraphAnalytics = ({ users }) => {
  const calculateMetrics = () => {
    if (users.length === 0) return {}

    const totalConnections = users.reduce((sum, u) => sum + u.friends.length, 0) / 2

    const avgConnections = users.reduce((sum, u) => sum + u.friends.length, 0) / users.length

    const mostConnected = users.reduce((prev, current) =>
      current.friends.length > prev.friends.length ? current : prev
    )

    const maxPossibleConnections = (users.length * (users.length - 1)) / 2
    const density = maxPossibleConnections > 0 ? (totalConnections / maxPossibleConnections * 100).toFixed(2) : 0

    const diameter = calculateNetworkDiameter(users)

    return {
      totalUsers: users.length,
      totalConnections: totalConnections,
      avgConnections: avgConnections.toFixed(2),
      maxConnections: mostConnected.friends.length,
      mostConnectedUser: mostConnected.name,
      density,
      diameter
    }
  }

  const calculateNetworkDiameter = (users) => {
    if (users.length <= 1) return 0
    
    let maxDistance = 0
    
    users.forEach(startUser => {
      const distances = bfs(startUser._id, users)
      const userDistances = Object.values(distances).filter(d => d !== Infinity)
      if (userDistances.length > 0) {
        maxDistance = Math.max(maxDistance, Math.max(...userDistances))
      }
    })
    
    return maxDistance === Infinity ? users.length - 1 : maxDistance
  }

  const bfs = (startId, users) => {
    const distances = {}
    users.forEach(u => distances[u._id] = Infinity)
    distances[startId] = 0

    const queue = [startId]
    
    while (queue.length > 0) {
      const userId = queue.shift()
      const user = users.find(u => u._id === userId)
      
      if (user) {
        user.friends.forEach(friendId => {
          if (distances[friendId] === Infinity) {
            distances[friendId] = distances[userId] + 1
            queue.push(friendId)
          }
        })
      }
    }
    
    return distances
  }

  const metrics = calculateMetrics()

  const MetricCard = ({ icon, label, value }) => (
    <div className="metric-card">
      <div className="metric-icon">{icon}</div>
      <div className="metric-content">
        <p className="metric-label">{label}</p>
        <p className="metric-value">{value}</p>
      </div>
    </div>
  )

  const InsightItem = ({ title, value, description }) => (
    <div className="insight-item">
      <h4>{title}</h4>
      <p className="insight-value">{value}</p>
      <p className="insight-description">{description}</p>
    </div>
  )

  return (
    <div className="analytics-container">
      <h2>Network Analytics</h2>

      <div className="metrics-grid">
        <MetricCard
          icon="👥"
          label="Total Users"
          value={metrics.totalUsers}
        />
        <MetricCard
          icon="🔗"
          label="Total Connections"
          value={metrics.totalConnections}
        />
        <MetricCard
          icon="📊"
          label="Avg Connections"
          value={metrics.avgConnections}
        />
        <MetricCard
          icon="⭐"
          label="Max Connections"
          value={metrics.maxConnections}
        />
        <MetricCard
          icon="🎯"
          label="Network Density"
          value={`${metrics.density}%`}
        />
        <MetricCard
          icon="📏"
          label="Network Diameter"
          value={metrics.diameter}
        />
      </div>

      <div className="insights-section">
        <h3>Network Insights</h3>
        <div className="insights-list">
          <InsightItem
            title="Most Connected User"
            value={metrics.mostConnectedUser}
            description={`${metrics.maxConnections} connections`}
          />
          <InsightItem
            title="Network Cohesion"
            value={metrics.density > 50 ? "High" : metrics.density > 20 ? "Medium" : "Low"}
            description={`${metrics.density}% density`}
          />
          <InsightItem
            title="Average Path Length"
            value={metrics.diameter}
            description="Maximum steps between any two users"
          />
        </div>
      </div>
    </div>
  )
}

export default GraphAnalytics
