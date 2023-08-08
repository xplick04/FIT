function toChunks(array)
{
	var results = [];

	while(Object.keys(array).length)
	{
		results.push(array.splice(0, 4));
	}
	return results;
};

export default toChunks;