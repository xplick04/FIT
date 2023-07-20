from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated
from datetime import date

from .models import ScheduleModel
from .serializers import ScheduleSerializer
from .utils import hasRole, checkSchedule

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def createSchedule(request):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')

	if not checkSchedule(request):
		return Response('Collision.')
	
	serializer = ScheduleSerializer(data=request.data)
	if serializer.is_valid():
		serializer.save()
		response = 'Schedule is created.'
	else:
		response = 'Schedule is not created.'
	return Response(response)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def listSchedule(request, pk):
	if (not hasRole(request, 'Caretaker')) and (not hasRole(request, 'Registered')):
		return Response('You do not have permission.')
	schedules = ScheduleModel.objects.filter(animal=pk)
	upcoming = schedules.filter(date__gte=date.today())
	serializer = ScheduleSerializer(upcoming, many=True)
	return Response(serializer.data)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteSchedule(request, pk):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	try:
		record = ScheduleModel.objects.get(id=pk)
		record.delete()
		response = 'Schedule is deleted.'
	except:
		response = 'Schedule is not deleted.'
	return Response(response)