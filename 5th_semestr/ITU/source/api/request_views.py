from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated

from .models import RequestModel
from .serializers import RequestSerializer
from .utils import hasRole

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def createRequest(request):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	serializer = RequestSerializer(data=request.data)
	if serializer.is_valid():
		serializer.save()
		response = 'Request is created.'
	else:
		response = 'Request is not created.'
	return Response(response)

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def listRequest(request):
	if not hasRole(request, 'Veterinarian'):
		return Response('You do not have permission.')
	requests_v = RequestModel.objects.all()
	serializer = RequestSerializer(requests_v, many=True)
	return Response(serializer.data)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteRequest(request, pk):
	if not hasRole(request, 'Veterinarian'):
		return Response('You do not have permission.')
	try:
		request_v = RequestModel.objects.get(id=pk)
		request_v.delete()
		response = 'Request is deleted.'
	except:
		response = 'Request is not deleted.'
	return Response(response)