# DisplayServer.py
# Assignment 2

# Author: Alessandro Lira

import socket, select, cPickle, threading, random, math, sys, os

from graphics import *

import platform

if platform.system() == 'Windows':
	import msvcrt
else:
	import termios, fcntl


class DisplayServer(threading.Thread):

	def __init__(self):
		threading.Thread.__init__(self)

		self.__myHostName = socket.gethostname()

		self.__colors = ['blue','red','green','cyan', 'magenta',
		'orange','purple','brown','gray','black']

		#[PLACEHOLDER] see if this runs any different without the 'Nones'
		self.__numVehicles = None

		self.__gvX = None
		self.__gvY = None
		self.__gvTheta = None

		self.__vehicleWaypoints = [] # one entry per vehicle; each entry is a list of (x,y) tuples

		self.__maxHistoryLength = 1000
		self.__histories = []

		self.__trace = True
		self.__demo = False #bolean used to determine is connected displayClient is running the demo

		self.initializeDisplay()

		self.quit = False

	def setVehicleData(self, numVehicles, gvX, gvY, gvTheta):
		self.__numVehicles = numVehicles
		self.__gvX = gvX
		self.__gvY = gvY
		self.__gvTheta = gvTheta

	def clear(self):
		self.__histories = []
		self.__vehicleWaypoints = []
		self.displayWindow.delete('all')

	def setTrace(self, value):
		self.__trace = value # should be a boolean!

	def setDemo(self, value):
		self.__demo = value

	def updateHistories(self):
		if (len(self.__histories) != self.__numVehicles):
			# reset histories to have proper length of vehicles
			self.__histories = []
			for i in range(self.__numVehicles):
				# add new vehicle history at end of history list
				self.__histories.append([])
				# add most recent vehicle position at beginning of that 'blank' history
				self.__histories[i].insert(0,(self.__gvX[i],self.__gvY[i]))
		else: # numVehicles still matches number of GV histories stored in histories
			if len(self.__histories[0]) < self.__maxHistoryLength:
				for i in range(len(self.__histories)):
					self.__histories[i].insert(0,(self.__gvX[i],self.__gvY[i]))
			else: # history length has reached max lenth, pop off oldest value
				for i in range(len(self.__histories)):
					self.__histories[i].pop()
					self.__histories[i].insert(0,(self.__gvX[i],self.__gvY[i]))

	def addVehicleWaypoints(self, vehicle_index, x_all, y_all):
		while len(self.__vehicleWaypoints) < vehicle_index+1:
			self.__vehicleWaypoints.append([])
		for i in range(len(x_all)):
			self.__vehicleWaypoints[vehicle_index].append((x_all[i]*5, y_all[i]*5))

	def repaint(self):

		self.displayWindow.delete('all')
		self.drawAxes()
		self.drawWaypoints()
		self.drawVehicles()
		if self.__trace:
			self.drawVehicleTrails()

		self.displayWindow.update()

	def initializeDisplay(self):
		hostIP = socket.gethostbyname(socket.gethostname())
		self.displayWindow = GraphWin('Display Client started on %s' % hostIP, 500, 500, False)
		self.displayWindow.setCoords(0,0,500,500)

		currentPos = 'Placeholder postion: (X,Y)'
		cPos = Text(Point(250,250),currentPos)
		cPos.draw(self.displayWindow)
		self.displayWindow.update()

	def drawAxes(self):
		origin = Circle(Point(0,0), 8)
		origin.setFill('black')
		origin.draw(self.displayWindow)
		xAxis = Line(Point(2,0), Point(450,00))
		xAxis.setFill('black')
		xAxis.setArrow('last')
		xAxis.draw(self.displayWindow)
		yAxis = Line(Point(2,0), Point(2,450))
		yAxis.setFill('black')
		yAxis.setArrow('last')
		yAxis.draw(self.displayWindow)
		N = 10
		for xi in range(N):
			x = 500/N*(xi+1)
			gridline = Line(Point(x,0), Point(x,500))
			gridline.setFill('gray')
			gridline.draw(self.displayWindow)
		for yi in range(N):
			y = 500/N*(yi+1)
			gridline = Line(Point(0,y), Point(500,y))
			gridline.setFill('gray')
			gridline.draw(self.displayWindow)

	def drawWaypoints(self):
		for vehicle_index in range(len(self.__vehicleWaypoints)):
			for (x,y) in self.__vehicleWaypoints[vehicle_index]:
				waypoint = Circle(Point(x,y), 4)
				waypoint.setFill(self.__colors[vehicle_index%10])
				waypoint.draw(self.displayWindow)

	def drawVehicles(self):
		for i in range(self.__numVehicles):
			posX = self.__gvX[i]*5
			posY = self.__gvY[i]*5
			theta = self.__gvTheta[i]
			headPoint = Point(posX,posY) # current location of GV
			# draw vehicle as arrow head
			tailPoint = Point(posX-math.cos((theta))*10,
				posY-math.sin((theta))*10)
			headLine = Line(tailPoint,headPoint)
			headLine.setFill(self.__colors[i%10])
			headLine.setArrow('last')
			headLine.draw(self.displayWindow)

		if self.__demo:
			str1 = 'If you can see two squares being traced by two vehicles then'
			str2 = 'Display Client has succefully connected to Display Server'
			text1 = Text(Point(250,452),str1)
			text2 = Text(Point(250,440),str2)
			text1.draw(self.displayWindow)
			text2.draw(self.displayWindow)

	def drawVehicleTrails(self):
		if len(self.__histories) == 0:
			return None

		for i in range(len(self.__histories)):
			xyPointList = []
			for xyTuple in self.__histories[i]:
				point = Point(xyTuple[0]*5,xyTuple[1]*5)
				xyPointList.append(point)

			# make a polygon object from a list of all the path vertices
			# of GV[i], then draw that polygon in the DisplayWindow
			xyPath = Polygon(xyPointList) #ckf: this is why we have that extra line....
			xyPath.setOutline(self.__colors[i%10])
			xyPath.draw(self.displayWindow)

	def startQuit(self):
		self.quit = True


class MessageListener(threading.Thread):

	def __init__(self, displayServer):
		threading.Thread.__init__(self)
		self.__displayServer = displayServer
		self.__connection_list = []
		self.__recv_buffer = 4096 # should be exponent of 2
		host = "0.0.0.0"
		self.__port = 1635
		self.__server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.__server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.__server_socket.bind((host,self.__port)) # bind socket to local address of machine at specified port #
		self.__server_socket.listen(10) # backlog value for max queued connections, must be atleast 0
		# Add server socket (this socket) to list of readable connections
		self.__connection_list.append(self.__server_socket)
		print "DisplayServer started on port " + str(self.__port)

	def run(self):
		while 1:
			# check to see if user has initiated quit sequence
			if self.__displayServer.quit:
				print 'ML: quit condition caught'
				break
			#print 'strted ds run top while loop'
			# Get the list of sockets which are ready to be read using 'select()'
			read_sockets, write_sockets, error_sockets = select.select(self.__connection_list,[],[])

			for sock in read_sockets:
				# New Connection
				if sock == self.__server_socket:
					# Handle the case in which there is a new connection recieved through server_socket
					sock_new, addr = self.__server_socket.accept()
					self.__connection_list.append(sock_new)
					print "Client (%s, %s) connected" % addr
				else:
					# proccess data received
					try:
						#In Windows, sometimes when a TCP program closes abruptly,
						# a "Connection reset by peer" exception will be thrown
						data = sock.recv(self.__recv_buffer)
						if data:
							message = cPickle.loads(data)
							# print 'MESSAGE', message
							if (len(message) == 1): # scenario where a command is given, NOT vehicle data
								if message[0] == 'clear':
									self.__displayServer.clear()
								elif message[0] == 'traceon':
									self.__displayServer.setTrace(True)
								elif message[0] == 'traceoff':
									self.__displayServer.setTrace(False)
								elif message[0] == 'demo':
									self.__displayServer.setDemo(True)

							elif (len(message) == 4): # length of vehicle data message
								self.__displayServer.setVehicleData(message[0],message[1],message[2],message[3])
								self.__displayServer.updateHistories()
								self.__displayServer.repaint()
								#time.sleep(.01)

							elif (len(message) == 3): # length of vehicle waypoint data message
								self.__displayServer.addVehicleWaypoints(message[0], message[1], message[2])
								#self.__displayServer.repaint()
								# time.sleep(.01)

							else:
								print 'Err: Message was not length 1, 3, or 4'

					except:
						raise
						print "DisplayClient (%s, %s) encountered an error" % (addr, random.randint(1,10))
						continue

		self.__server_socket.close()

# Adapted from python documentation at:
# https://docs.python.org/2/faq/library.html#how-do-i-get-a-single-keypress-at-a-time
class KeyListenerUnix(threading.Thread):
	def __init__(self, displayServer):
		threading.Thread.__init__(self)
		self.__displayServer = displayServer
		self.__fd = sys.stdin.fileno()
		self.__oldterm = termios.tcgetattr(self.__fd)
		self.__newattr = termios.tcgetattr(self.__fd)
		self.__newattr[3] = self.__newattr[3] & ~termios.ICANON & ~termios.ECHO
		termios.tcsetattr(self.__fd, termios.TCSANOW, self.__newattr)
		self.__oldflags = fcntl.fcntl(self.__fd, fcntl.F_GETFL)
		fcntl.fcntl(self.__fd, fcntl.F_SETFL, self.__oldflags | os.O_NONBLOCK)

	def run(self):
		try:
			while 1:
				if self.__displayServer.quit:
					print 'KL: quit condition caught'
					break
				try:
					c = sys.stdin.read(1)
					if (c=='q' or c=='Q'):
						self.__displayServer.startQuit()
				except IOError: pass
		finally:
			termios.tcsetattr(self.__fd, termios.TCSAFLUSH, self.__oldterm)
			fcntl.fcntl(self.__fd, fcntl.F_SETFL, self.__oldflags)

class KeyListenerWindows(threading.Thread):
	def __init__(self, displayServer):
		threading.Thread.__init__(self)
		self.__displayServer = displayServer

	def run(self):
		while 1:
			if self.__displayServer.quit:
				print 'KL: quit condition caught'
				break
			try:
				c = msvcrt.getch()
				print "Got character", repr(c)
				if (c=='q' or c=='Q'):
					self.__displayServer.startQuit()
					#break
			except IOError: pass

if __name__ == '__main__':
	ds = DisplayServer()
	ml = MessageListener(ds)
	if platform.system() == 'Windows':
		kl = KeyListenerWindows(ds)
	else:
		kl = KeyListenerUnix(ds)
	kl.start()
	ml.run()
	kl.join()
