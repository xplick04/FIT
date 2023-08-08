from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated

from .models import ReservationModel, AnimalModel, ScheduleModel
from .serializers import ReservationSerializer
from .utils import hasRole, checkCollision

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def createReservation(request):
	if not hasRole(request, 'Registered'):
		return Response('You do not have permission.')

	if not checkCollision(request):
		return Response('Collision.')

	serializer = ReservationSerializer(data=request.data)
	if serializer.is_valid():
		serializer.save()
		response = 'Reservation is created.'
	else:
		response = 'Reservation is not created.'
	return Response(response)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def listReservation(request, pk):
	if (not hasRole(request, 'Caretaker')) and (not hasRole(request, 'Registered')):
		return Response('You do not have permission.')
	schedules = ReservationModel.objects.filter(schedule=pk)
	serializer = ReservationSerializer(schedules, many=True)
	return Response(serializer.data)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def myWalks(request):
	if not hasRole(request, 'Registered'):
		return Response('You do not have permission.')
	user_id =request.user.id
	reservations = ReservationModel.objects.filter(user=user_id)
	response = []
	for reservation in reservations:
		schedule = ScheduleModel.objects.get(id=reservation.schedule.id)
		animal = AnimalModel.objects.get(id=schedule.animal.id)
		dict = {
			'schedule':schedule.date,
			'animal':animal.name,
			'r_from':reservation.r_from,
			'r_to':reservation.r_to,
			'id':reservation.id,
			'confirmed':reservation.confirmed
		}
		response.append(dict)
	return Response(response)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteReservation(request, pk):
	reservation = ReservationModel.objects.get(id=pk)
	user = reservation.user
	if (not hasRole(request, 'Caretaker')) and (request.user.username != str(user)):
		return Response('You do not have permission.')
	try:
		reservation.delete()
		response = 'Reservation is deleted.'
	except:
		response = 'Reservation is not deleted.'
	return Response(response)

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def updateReservation(request, pk):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	data = request.data
	try:
		confirmed = data['confirmed']
		borrowed = data['borrowed']
		returned = data['returned']
	except:
		return Response('Reservation is not updated.')
	
	reservation = ReservationModel.objects.get(id=pk)
	reservation.confirmed = confirmed
	reservation.borrowed = borrowed
	reservation.returned = returned
	reservation.save()
	return Response('User is updated.')