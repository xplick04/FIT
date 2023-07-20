from .models import UserModel, ScheduleModel, ReservationModel
from datetime import datetime

def hasRole(request, role):
	if request.user.is_authenticated:
		if request.user.type == role:
			return True
	return False

def getUsername(pk):
	try:
		user = UserModel.objects.get(id=pk)
	except:
		return ''
	username = user.username
	return username

def checkCollision(request):
	pk = request.data.get('schedule')
	schedule = ScheduleModel.objects.get(id=pk)
	reservations = ReservationModel.objects.filter(schedule=pk)
	r_from = datetime.strptime(request.data.get('r_from'), '%H:%M').time()
	r_to = datetime.strptime(request.data.get('r_to'), '%H:%M').time()
	s_from = schedule.s_from
	s_to = schedule.s_to

	if r_from > r_to:
		return False

	if r_from < s_from or r_to > s_to:
		return False

	for reservation in reservations:
		if reservation.r_from < r_from < reservation.r_to:
			return False
		elif r_to > reservation.r_from and r_to < reservation.r_to:
			return False
		elif r_from <= reservation.r_from and r_to >= reservation.r_to:
			return False
	
	return True

def checkSchedule(request):
	animal_id = request.data.get('animal')
	schedules = ScheduleModel.objects.filter(id=animal_id)
	s_date = datetime.strptime(request.data.get('date'), '%Y-%m-%d').date()
	s_from = datetime.strptime(request.data.get('s_from'), '%H:%M').time()
	s_to = datetime.strptime(request.data.get('s_to'), '%H:%M').time()
	
	if s_from > s_to:
		return False

	for schedule in schedules:
		if s_date == schedule.date:
			return False

	
	return True