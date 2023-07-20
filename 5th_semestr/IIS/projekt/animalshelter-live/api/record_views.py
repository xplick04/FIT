from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated

from .models import RecordModel
from .serializers import RecordSerializer
from .utils import hasRole

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def createRecord(request):
	if not hasRole(request, 'Veterinarian'):
		return Response('You do not have permission.')
	serializer = RecordSerializer(data=request.data)
	if serializer.is_valid():
		serializer.save()
		response = 'Record is created.'
	else:
		response = 'Record is not created.'
	return Response(response)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def listRecord(request, pk):
	if not hasRole(request, 'Veterinarian'):
		return Response('You do not have permission.')
	requests = RecordModel.objects.filter(animal=pk)
	serializer = RecordSerializer(requests, many=True)
	return Response(serializer.data)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteRecord(request, pk):
	if not hasRole(request, 'Veterinarian'):
		return Response('You do not have permission.')
	try:
		record = RecordModel.objects.get(id=pk)
		record.delete()
		response = 'Record is deleted.'
	except:
		response = 'Record is not deleted.'
	return Response(response)